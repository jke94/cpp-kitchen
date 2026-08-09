#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

/**
 * @brief The Task Engine API provides a simple interface for submitting tasks to a thread pool and handling exceptions.
 */
namespace taskEngineApi
{
    /**
     * @brief Exception handler interface.
     *
     * Clients can implement this interface to handle exceptions thrown by tasks.
     * The engine will call OnException when a task throws an exception.
     */
    class IExceptionHandler
    {
    public:

        /**
         * @brief Virtual destructor for polymorphic base class.
         */
        virtual ~IExceptionHandler() = default;

        /**
         * @brief Called when a task throws.
         * @note Must be noexcept and thread-safe. Violations are swallowed
         *       by the worker so that a bad handler cannot kill the process.
         */
        virtual void OnException(
            std::exception_ptr eptr,
            const std::string& context
        ) noexcept = 0;

    protected:
        IExceptionHandler() = default;
        IExceptionHandler(const IExceptionHandler&) = default;
        IExceptionHandler& operator=(const IExceptionHandler&) = default;
    };

    /**
     * @brief Task metrics.
     *
     * Invariant (after all outstanding work has finished):
     *   submitted == completed + failed
     *
     * - completed : tasks that finished without throwing
     * - failed    : tasks that threw (observed by the worker)
     * - totalLatencyMs / averageLatencyMs : include BOTH success and failure
     */
    struct TaskMetrics
    {
        std::uint64_t submitted         = 0;
        std::uint64_t completed         = 0;  // successful only
        std::uint64_t failed            = 0;
        double        totalLatencyMs    = 0.0; // success + failure
        double        averageLatencyMs  = 0.0; // total / (completed + failed)
        double        tasksPerSecond    = 0.0; // (completed + failed) / elapsed
        std::chrono::steady_clock::time_point startTime;
    };

    class IEngine
    {
    public:

        /**
         * @brief Virtual destructor for polymorphic base class.
         */
        virtual ~IEngine() = default;

        /**
         * @brief Fire-and-forget + polymorphic queue hook.
         * @throws std::invalid_argument if task is empty
         * @throws std::runtime_error if the engine is stopped
         */
        virtual void SubmitDetached(std::function<void()> task) = 0;

        /**
         * @brief Primary API: submit callable, get future<T>.
         *
         * Exceptions thrown by the callable:
         *   1) are stored in the returned future (future.get() rethrows)
         *   2) are observed by the worker → OnException + metrics.failed
         */
        template <typename F, typename... Args>
        auto Submit(F&& f, Args&&... args)
            -> std::future<std::invoke_result_t<std::decay_t<F>, Args...>>;

        /**
         * @brief Blocks until outstanding tasks (queued + executing) == 0.
         */
        virtual void WaitForIdle() = 0;

        /**
         * @brief Graceful stop: no new tasks; workers exit when queue is empty.
         *        Tasks already dequeued finish. Join all workers.
         */
        virtual void Stop() = 0;

        virtual std::size_t GetThreadCount() const = 0;

        /**
         * @brief Outstanding tasks = queued + currently executing.
         */
        virtual std::size_t GetPendingTaskCount() const = 0;

        /**
         * Snapshot only. NOT safe as: if (IsRunning()) Submit(...);
         * TOCTOU with Stop(). Submit/SubmitDetached validate internally.
         */
        virtual bool IsRunning() const = 0;

        virtual TaskMetrics GetMetrics() const = 0;

        /**
         * Best-effort reset. Prefer calling when idle.
         */
        virtual void ResetMetrics() = 0;

    protected:
        IEngine() = default;
        IEngine(const IEngine&) = default;
        IEngine& operator=(const IEngine&) = default;
    };

    // -----------------------------------------------------------------
    // Template Submit — exceptions propagate to BOTH future and worker
    // -----------------------------------------------------------------
    template <typename F, typename... Args>
    auto IEngine::Submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<std::decay_t<F>, Args...>>
    {
        using ReturnType = std::invoke_result_t<std::decay_t<F>, Args...>;

        auto promise = std::make_shared<std::promise<ReturnType>>();
        std::future<ReturnType> future = promise->get_future();

        auto work = [promise,
                     fn = std::decay_t<F>(std::forward<F>(f)),
                     tup = std::make_tuple(std::forward<Args>(args)...)]() mutable
        {
            try
            {
                if constexpr (std::is_void_v<ReturnType>)
                {
                    std::apply(std::move(fn), std::move(tup));
                    promise->set_value();
                }
                else
                {
                    promise->set_value(
                        std::apply(std::move(fn), std::move(tup))
                    );
                }
            }
            catch (...)
            {
                promise->set_exception(std::current_exception());
                throw;
            }
        };

        SubmitDetached(std::function<void()>(std::move(work)));

        return future;
    }

    std::unique_ptr<IEngine> createEngine(
        std::size_t numThreads,
        std::shared_ptr<IExceptionHandler> exceptionHandler
    );

} // namespace taskEngineApi

namespace taskEngineApiPrivate
{
    class NullExceptionHandler final : public taskEngineApi::IExceptionHandler
    {
    public:
        void OnException(
            std::exception_ptr eptr,
            const std::string& context
        ) noexcept override;
    };

    /**
     * @brief Concrete engine. Depend on IEngine from client code.
     *
     * Public surface is limited to construction/destruction.
     * All IEngine overrides are private: still reachable via IEngine*,
     * but not callable on an Engine* / Engine& directly.
     */
    class Engine final : public taskEngineApi::IEngine
    {
    public:
        explicit Engine(
            std::size_t numThreads,
            std::shared_ptr<taskEngineApi::IExceptionHandler> exceptionHandler
        );

        ~Engine() override;

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

    private:
        // --- IEngine overrides (private: use through IEngine*) ---
        void SubmitDetached(std::function<void()> task) override;
        void WaitForIdle() override;
        void Stop() override;
        std::size_t GetThreadCount() const override;
        std::size_t GetPendingTaskCount() const override;
        bool IsRunning() const override;
        taskEngineApi::TaskMetrics GetMetrics() const override;
        void ResetMetrics() override;

        void WorkerLoop();

        std::shared_ptr<taskEngineApi::IExceptionHandler>  exceptionHandler;
        const std::size_t threadCount;

        // Shared mutable state protected by mutex, except stopFlag
        // (atomic for lock-free IsRunning; written under mutex in Stop).
        mutable std::mutex                  mutex;
        std::condition_variable             cv;
        std::condition_variable             idleCv;
        std::queue<std::function<void()>>   tasks;

        std::vector<std::thread>            workers;
        std::atomic<bool>                   stopFlag{false};
        std::size_t                         pendingTasks{0};

        taskEngineApi::TaskMetrics metrics;
    };

} // namespace taskEngineApiPrivate

// ============================================================
// Client: HTTP GET / POST simulation
// ============================================================
namespace taskEngineClient
{
    struct HttpResponse
    {
        int         statusCode  = 0;
        std::string body;
        std::string contentType;
        int         requestId   = 0;
    };

    struct PostResult
    {
        int         statusCode  = 0;
        std::string location;
        std::string body;
        int         requestId   = 0;
    };

    const int MIN_LATENCY_MS     = 80;
    const int MAX_LATENCY_MS     = 120;
    const int FAILURE_PERCENTAGE = 7;

    HttpResponse httpGet(int id);
    PostResult   httpPost(int id, const std::string& payload);

    class LoggingExceptionHandler final : public taskEngineApi::IExceptionHandler
    {
    public:
        void OnException(
            std::exception_ptr eptr,
            const std::string& context
        ) noexcept override;
    };

} // namespace taskEngineClient

int main(int /*argc*/, char* /*argv*/[])
{
    // Miniumum namespace access for client code.
    using namespace taskEngineApi;
    using namespace taskEngineClient;

    constexpr std::size_t NUM_THREADS = 6;
    constexpr int         BATCH_SIZE  = 20;
    constexpr int         NUM_BATCHES = 4;

    auto exceptionHandler = std::make_shared<LoggingExceptionHandler>();

    // Create engine depenending of the abstraction.
    std::unique_ptr<IEngine> engine = createEngine(NUM_THREADS, exceptionHandler);

    std::cout << "TaskEngine started with " << engine->GetThreadCount()
              << " worker threads\n"
              << "Failure simulation: " << FAILURE_PERCENTAGE << "%\n"
              << "Latency range: [" << MIN_LATENCY_MS << "–" << MAX_LATENCY_MS << "] ms\n\n";

    for (int batch = 0; batch < NUM_BATCHES; ++batch)
    {
        std::cout << ">>> Batch " << (batch + 1) << "/" << NUM_BATCHES
                  << " — submitting " << BATCH_SIZE << " GET + "
                  << BATCH_SIZE << " POST ...\n";

        std::vector<std::future<HttpResponse>> getFutures;
        std::vector<std::future<PostResult>>   postFutures;
        getFutures.reserve(static_cast<std::size_t>(BATCH_SIZE));
        postFutures.reserve(static_cast<std::size_t>(BATCH_SIZE));

        for (int i = 0; i < BATCH_SIZE; ++i)
        {
            const int requestId = batch * BATCH_SIZE + i;

            getFutures.push_back(
                engine->Submit([requestId]() -> HttpResponse {
                    return httpGet(requestId);
                })
            );

            postFutures.push_back(
                engine->Submit([requestId]() -> PostResult {
                    return httpPost(requestId, "{\"action\":\"create\",\"id\":"
                                    + std::to_string(requestId) + "}");
                })
            );
        }

        engine->WaitForIdle();

        int getOk = 0, getErr = 0;
        for (auto& fut : getFutures)
        {
            try
            {
                HttpResponse resp = fut.get();
                if (resp.statusCode >= 200 && resp.statusCode < 300)
                {
                    ++getOk;
                }
                else
                {
                    ++getErr;
                }
            }
            catch (const std::exception&)
            {
                ++getErr;
            }
        }

        int postOk = 0, postErr = 0;
        for (auto& fut : postFutures)
        {
            try
            {
                PostResult resp = fut.get();
                if (resp.statusCode >= 200 && resp.statusCode < 300)
                {
                    ++postOk;
                }
                else
                {
                    ++postErr;
                }
            }
            catch (const std::exception&)
            {
                ++postErr;
            }
        }

        const TaskMetrics m = engine->GetMetrics();
        const std::uint64_t finished = m.completed + m.failed;

        std::cout << "--- Metrics after batch " << (batch + 1) << " ---\n"
                  << "  Submitted      : " << m.submitted << "\n"
                  << "  Completed (ok) : " << m.completed << "\n"
                  << "  Failed         : " << m.failed
                  << " (approx. " << (m.submitted ? m.failed * 100.0 / m.submitted : 0.0) << "%)\n"
                  << "  Finished total : " << finished
                  << " (invariant submitted==completed+failed: "
                  << (m.submitted == finished ? "OK" : "BROKEN") << ")\n"
                  << "  Avg Latency    : " << m.averageLatencyMs << " ms (all finished tasks)\n"
                  << "  Throughput     : " << m.tasksPerSecond << " tasks/s\n"
                  << "  Pending        : " << engine->GetPendingTaskCount() << "\n"
                  << "  GET  OK/ERR    : " << getOk << " / " << getErr << "\n"
                  << "  POST OK/ERR    : " << postOk << " / " << postErr << "\n\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "Stopping TaskEngine...\n";
    engine->Stop();

    const TaskMetrics finalMetrics = engine->GetMetrics();
    const std::uint64_t finished = finalMetrics.completed + finalMetrics.failed;

    std::cout << "\n=== Final metrics ===\n"
              << "Total submitted : " << finalMetrics.submitted << "\n"
              << "Total completed : " << finalMetrics.completed << "\n"
              << "Total failed    : " << finalMetrics.failed
              << " (approx. "
              << (finalMetrics.submitted
                      ? finalMetrics.failed * 100.0 / finalMetrics.submitted
                      : 0.0)
              << "%)\n"
              << "Finished total  : " << finished
              << " (invariant: "
              << (finalMetrics.submitted == finished ? "OK" : "BROKEN") << ")\n"
              << "Avg latency     : " << finalMetrics.averageLatencyMs << " ms\n"
              << "Avg throughput  : " << finalMetrics.tasksPerSecond << " tasks/s\n";

    return 0;
}

// ============================================================
// Engine implementation
// ============================================================
namespace taskEngineApi
{
    std::unique_ptr<IEngine> _engine = nullptr;

    std::unique_ptr<IEngine> createEngine(
        std::size_t numThreads,
        std::shared_ptr<IExceptionHandler> exceptionHandler
    )
    {
        if(_engine)
        {
            throw std::runtime_error("TaskEngine already created");
        }

        _engine = std::make_unique<taskEngineApiPrivate::Engine>(
            numThreads,
            std::move(exceptionHandler)
        );

        return std::move(_engine);
    }

} // namespace taskEngineApi

// ============================================================
// Client implementation
// ============================================================
namespace taskEngineClient
{
    HttpResponse httpGet(int id)
    {
        static_assert(MIN_LATENCY_MS >= 0, "MIN_LATENCY_MS must be >= 0");
        static_assert(MAX_LATENCY_MS >= MIN_LATENCY_MS, "MAX_LATENCY_MS must be >= MIN_LATENCY_MS");
        static_assert(FAILURE_PERCENTAGE >= 0 && FAILURE_PERCENTAGE <= 100,
                      "FAILURE_PERCENTAGE must be in [0, 100]");

        const int range = MAX_LATENCY_MS - MIN_LATENCY_MS + 1;
        const auto latency = std::chrono::milliseconds(MIN_LATENCY_MS + (id % range));
        std::this_thread::sleep_for(latency);

        if (FAILURE_PERCENTAGE > 0 && (id % 100) < FAILURE_PERCENTAGE)
        {
            throw std::runtime_error(
                "HTTP GET error (simulated) for request id=" + std::to_string(id)
            );
        }

        HttpResponse resp;
        resp.statusCode  = 200;
        resp.contentType = "application/json";
        resp.requestId   = id;
        resp.body        = std::string("{\"id\":")
                         + std::to_string(id)
                         + ",\"method\":\"GET\""
                         + ",\"status\":\"ok\""
                         + ",\"payload\":\"sample-data-"
                         + std::to_string(id)
                         + "\"}";
        return resp;
    }

    PostResult httpPost(int id, const std::string& payload)
    {
        const int range = MAX_LATENCY_MS - MIN_LATENCY_MS + 1;
        const auto latency = std::chrono::milliseconds(
            MIN_LATENCY_MS + ((id * 3) % range)
        );
        std::this_thread::sleep_for(latency);

        if (FAILURE_PERCENTAGE > 0 && ((id + 3) % 100) < FAILURE_PERCENTAGE)
        {
            throw std::runtime_error(
                "HTTP POST error (simulated) for request id=" + std::to_string(id)
            );
        }

        PostResult resp;
        resp.statusCode = 201;
        resp.requestId  = id;
        resp.location   = "/resources/" + std::to_string(id);
        resp.body       = std::string("{\"id\":")
                        + std::to_string(id)
                        + ",\"method\":\"POST\""
                        + ",\"status\":\"created\""
                        + ",\"echo\":"
                        + payload
                        + "}";
        return resp;
    }

    void LoggingExceptionHandler::OnException(
        std::exception_ptr eptr,
        const std::string& context
    ) noexcept
    {
        try
        {
            if (eptr)
            {
                std::rethrow_exception(eptr);
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "[EXCEPTION][" << context << "] " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "[EXCEPTION][" << context << "] Unknown exception" << std::endl;
        }
    }

} // namespace taskEngineClient


namespace taskEngineApiPrivate
{
    using namespace taskEngineApi;

    Engine::Engine(
        std::size_t numThreads,
        std::shared_ptr<IExceptionHandler> exceptionHandler
    )
        : threadCount(numThreads)
        , stopFlag(false)
        , pendingTasks(0)
    {
        if (numThreads == 0)
        {
            throw std::invalid_argument("TaskEngine: numThreads must be > 0");
        }

        if (exceptionHandler)
        {
            this->exceptionHandler = std::move(exceptionHandler);
        }
        else
        {
            this->exceptionHandler = std::make_shared<NullExceptionHandler>();
        }

        metrics.startTime = std::chrono::steady_clock::now();

        workers.reserve(numThreads);
        for (std::size_t i = 0; i < numThreads; ++i)
        {
            workers.emplace_back([this] {
                WorkerLoop();
            });
        }
    }

    Engine::~Engine()
    {
        Stop();
    }

    void Engine::SubmitDetached(std::function<void()> task)
    {
        if (!task)
        {
            throw std::invalid_argument("TaskEngine::SubmitDetached: empty task is not allowed");
        }

        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopFlag.load(std::memory_order_relaxed))
            {
                throw std::runtime_error("TaskEngine is stopped");
            }

            tasks.push(std::move(task));
            ++pendingTasks;
            ++metrics.submitted;
        }
        cv.notify_one();
    }

    void Engine::WaitForIdle()
    {
        std::unique_lock<std::mutex> lock(mutex);
        idleCv.wait(lock, [this] {
            return pendingTasks == 0;
        });
    }

    void Engine::Stop()
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopFlag.load(std::memory_order_relaxed))
            {
                return;
            }
            stopFlag.store(true, std::memory_order_release);
        }

        cv.notify_all();

        for (auto& t : workers)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
        workers.clear();
    }

    std::size_t Engine::GetThreadCount() const
    {
        return threadCount;
    }

    std::size_t Engine::GetPendingTaskCount() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return pendingTasks;
    }

    bool Engine::IsRunning() const
    {
        return !stopFlag.load(std::memory_order_acquire);
    }

    TaskMetrics Engine::GetMetrics() const
    {
        std::lock_guard<std::mutex> lock(mutex);

        TaskMetrics m = metrics;

        const std::uint64_t finished = m.completed + m.failed;
        if (finished > 0)
        {
            m.averageLatencyMs = m.totalLatencyMs / static_cast<double>(finished);
        }

        const auto now = std::chrono::steady_clock::now();
        const double elapsedSec = std::chrono::duration<double>(now - m.startTime).count();

        if (elapsedSec > 0.0)
        {
            m.tasksPerSecond = static_cast<double>(finished) / elapsedSec;
        }

        return m;
    }

    void Engine::ResetMetrics()
    {
        std::lock_guard<std::mutex> lock(mutex);
        metrics = TaskMetrics{};
        metrics.startTime = std::chrono::steady_clock::now();
    }

    void Engine::WorkerLoop()
    {
        while (true)
        {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(mutex);

                cv.wait(lock, [this] {
                    return stopFlag.load(std::memory_order_relaxed) || !tasks.empty();
                });

                if (stopFlag.load(std::memory_order_relaxed) && tasks.empty())
                {
                    return;
                }

                task = std::move(tasks.front());
                tasks.pop();
            }

            const auto start = std::chrono::steady_clock::now();
            bool success = true;

            try
            {
                task();
            }
            catch (...)
            {
                success = false;

                try
                {
                    exceptionHandler->OnException(
                        std::current_exception(),
                        "TaskEngine::Worker"
                    );
                }
                catch (...)
                {
                    // Handler violated noexcept — keep worker alive
                }
            }

            const auto end = std::chrono::steady_clock::now();
            const double latencyMs =
                std::chrono::duration<double, std::milli>(end - start).count();

            {
                std::lock_guard<std::mutex> lock(mutex);

                metrics.totalLatencyMs += latencyMs;

                if (success)
                {
                    ++metrics.completed;
                }
                else
                {
                    ++metrics.failed;
                }

                --pendingTasks;
                if (pendingTasks == 0)
                {
                    idleCv.notify_all();
                }
            }
        }
    }

    void NullExceptionHandler::OnException(
        std::exception_ptr /*eptr*/,
        const std::string& context
    ) noexcept
    {
        std::cerr << "[NullExceptionHandler] Exception in context: " << context << std::endl;
    }

} // namespace taskEngineApiPrivate