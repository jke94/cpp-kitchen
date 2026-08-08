#include "TaskEngine.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <stdexcept>

struct TaskEngine::Impl
{
    explicit Impl(std::size_t numThreads,
                  std::shared_ptr<IExceptionHandler> handler)
        : exceptionHandler(handler ? std::move(handler)
                                   : std::make_shared<NullExceptionHandler>())
        , stopFlag(false)
        , pendingTasks(0)
    {
        if (numThreads == 0)
            throw std::invalid_argument("TaskEngine: numThreads must be > 0");

        metrics.startTime = std::chrono::steady_clock::now();

        workers.reserve(numThreads);
        for (std::size_t i = 0; i < numThreads; ++i)
        {
            workers.emplace_back([this] { WorkerLoop(); });
        }
    }

    ~Impl()
    {
        Stop();
    }

    void Submit(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopFlag)
                throw std::runtime_error("TaskEngine is stopped");

            tasks.push(std::move(task));
            ++pendingTasks;
            ++metrics.submitted;
        }
        cv.notify_one();
    }

    void WaitForIdle()
    {
        std::unique_lock<std::mutex> lock(mutex);
        idleCv.wait(lock, [this] { return pendingTasks == 0; });
    }

    void Stop()
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopFlag)
                return;
            stopFlag = true;
        }

        cv.notify_all();

        for (auto& t : workers)
        {
            if (t.joinable())
                t.join();
        }
        workers.clear();
    }

    std::size_t GetThreadCount() const
    {
        return workers.size();
    }

    std::size_t GetPendingTaskCount() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return pendingTasks;
    }

    bool IsRunning() const
    {
        return !stopFlag;
    }

    TaskMetrics GetMetrics() const
    {
        std::lock_guard<std::mutex> lock(mutex);

        TaskMetrics m = metrics;

        if (m.completed > 0)
            m.averageLatencyMs = m.totalLatencyMs / static_cast<double>(m.completed);

        const auto now = std::chrono::steady_clock::now();
        const double elapsedSec = std::chrono::duration<double>(now - m.startTime).count();

        if (elapsedSec > 0.0)
            m.tasksPerSecond = static_cast<double>(m.completed) / elapsedSec;

        return m;
    }

    void ResetMetrics()
    {
        std::lock_guard<std::mutex> lock(mutex);
        metrics = TaskMetrics{};
        metrics.startTime = std::chrono::steady_clock::now();
    }

private:
    void WorkerLoop()
    {
        while (true)
        {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(mutex);

                cv.wait(lock, [this] {
                    return stopFlag || !tasks.empty();
                });

                if (stopFlag && tasks.empty())
                    return;

                task = std::move(tasks.front());
                tasks.pop();
            }

            // ---- Medición de latencia + ejecución segura ----
            const auto start = std::chrono::steady_clock::now();
            bool success = true;

            try
            {
                task();
            }
            catch (...)
            {
                success = false;
                exceptionHandler->OnException(std::current_exception(), "TaskEngine::Worker");
            }

            const auto end = std::chrono::steady_clock::now();
            const double latencyMs =
                std::chrono::duration<double, std::milli>(end - start).count();

            {
                std::lock_guard<std::mutex> lock(mutex);

                if (success)
                {
                    ++metrics.completed;
                    metrics.totalLatencyMs += latencyMs;
                }
                else
                {
                    ++metrics.failed;
                }

                --pendingTasks;
                if (pendingTasks == 0)
                    idleCv.notify_all();
            }
        }
    }

    // ---- Estado interno ----
    std::shared_ptr<IExceptionHandler> exceptionHandler;

    mutable std::mutex                  mutex;
    std::condition_variable             cv;
    std::condition_variable             idleCv;
    std::queue<std::function<void()>>   tasks;

    std::vector<std::thread>            workers;
    std::atomic<bool>                   stopFlag;
    std::size_t                         pendingTasks;

    TaskMetrics                         metrics;   // protegido por mutex
};

// ============================================================
// TaskEngine public API
// ============================================================

TaskEngine::TaskEngine(std::size_t numThreads,
                       std::shared_ptr<IExceptionHandler> exceptionHandler)
    : pImpl(std::make_unique<Impl>(numThreads, std::move(exceptionHandler)))
{
}

TaskEngine::~TaskEngine() = default;

void TaskEngine::Submit(std::function<void()> task)
{
    pImpl->Submit(std::move(task));
}

void TaskEngine::WaitForIdle()
{
    pImpl->WaitForIdle();
}

void TaskEngine::Stop()
{
    pImpl->Stop();
}

std::size_t TaskEngine::GetThreadCount() const
{
    return pImpl->GetThreadCount();
}

std::size_t TaskEngine::GetPendingTaskCount() const
{
    return pImpl->GetPendingTaskCount();
}

bool TaskEngine::IsRunning() const
{
    return pImpl->IsRunning();
}

TaskMetrics TaskEngine::GetMetrics() const
{
    return pImpl->GetMetrics();
}

void TaskEngine::ResetMetrics()
{
    pImpl->ResetMetrics();
}
