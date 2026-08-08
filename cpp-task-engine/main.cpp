#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

/**
 * @brief TaskEngine API definition
 */
namespace taskEngine
{
    /**
     * @brief Interface para manejar excepciones lanzadas por las tareas.
     * Se inyecta por constructor → Dependency Injection.
     */
    class IExceptionHandler
    {
    public:
        virtual ~IExceptionHandler() = default;

        /**
         * Llamado cuando una tarea lanza una excepción.
         * Debe ser noexcept y thread-safe.
         */
        virtual void OnException(
            std::exception_ptr eptr,
            const std::string& context = ""
        ) noexcept = 0;
    };

    /**
     * Implementación por defecto (no hace nada).
     * Útil cuando no se quiere inyectar un handler real.
     */
    class NullExceptionHandler final : public IExceptionHandler
    {
    public:
        void OnException(std::exception_ptr, const std::string& = "") noexcept override
        {
        }
    };

    // ============================================================
    // Métricas
    // ============================================================

    struct TaskMetrics
    {
        std::uint64_t submitted        = 0;   // Tareas enviadas
        std::uint64_t completed        = 0;   // Tareas finalizadas con éxito
        std::uint64_t failed           = 0;   // Tareas que lanzaron excepción
        double        totalLatencyMs   = 0.0; // Suma de latencias (para media)
        double        averageLatencyMs = 0.0;
        double        tasksPerSecond   = 0.0;
        std::chrono::steady_clock::time_point startTime;
    };

    // ============================================================
    // Interface principal
    // ============================================================

    class ITaskEngine
    {
    public:
        virtual ~ITaskEngine() = default;

        virtual void Submit(std::function<void()> task) = 0;

        template <typename F, typename... Args>
        auto SubmitWithResult(F&& f, Args&&... args)
            -> std::future<std::invoke_result_t<std::decay_t<F>, Args...>>;

        virtual void WaitForIdle() = 0;
        virtual void Stop() = 0;

        virtual std::size_t GetThreadCount() const = 0;
        virtual std::size_t GetPendingTaskCount() const = 0;
        virtual bool IsRunning() const = 0;

        // ----- APIs de métricas -----
        virtual TaskMetrics GetMetrics() const = 0;
        virtual void ResetMetrics() = 0;
    };

    // ============================================================
    // Implementación concreta (sin PImpl)
    // ============================================================

    class TaskEngine : public ITaskEngine
    {
    public:
        /**
         * @param numThreads         Número de worker threads (> 0)
         * @param exceptionHandler   Handler de excepciones (opcional, DI)
         */
        explicit TaskEngine(std::size_t numThreads,
                            std::shared_ptr<IExceptionHandler> exceptionHandler = nullptr);

        ~TaskEngine() override;

        TaskEngine(const TaskEngine&) = delete;
        TaskEngine& operator=(const TaskEngine&) = delete;
        TaskEngine(TaskEngine&&) = delete;
        TaskEngine& operator=(TaskEngine&&) = delete;

        void Submit(std::function<void()> task) override;

        void WaitForIdle() override;
        void Stop() override;

        std::size_t GetThreadCount() const override;
        std::size_t GetPendingTaskCount() const override;
        bool IsRunning() const override;

        TaskMetrics GetMetrics() const override;
        void ResetMetrics() override;

    private:
        void WorkerLoop();

        // ---- Estado interno ----
        std::shared_ptr<IExceptionHandler>  exceptionHandler;

        mutable std::mutex                  mutex;
        std::condition_variable             cv;
        std::condition_variable             idleCv;
        std::queue<std::function<void()>>   tasks;

        std::vector<std::thread>            workers;
        std::atomic<bool>                   stopFlag{false};
        std::size_t                         pendingTasks{0};

        TaskMetrics                         metrics;
    };

    // -----------------------------------------------------------------
    // Template implementation (debe estar visible)
    // -----------------------------------------------------------------
    template <typename F, typename... Args>
    auto ITaskEngine::SubmitWithResult(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<std::decay_t<F>, Args...>>
    {
        using ReturnType = std::invoke_result_t<std::decay_t<F>, Args...>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<ReturnType> future = task->get_future();

        Submit([task]() {
            (*task)();
        });

        return future;
    }

} // namespace taskEngine

using namespace taskEngine;

// ============================================================
// Handler de excepciones de ejemplo
// ============================================================
class LoggingExceptionHandler final : public IExceptionHandler
{
public:
    void OnException(std::exception_ptr eptr, const std::string& context) noexcept override
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
};

// ============================================================
// Simulación de una petición HTTP
// ============================================================
void FetchData(int id)
{
    // Simulamos latencia variable de red
    const auto latency = std::chrono::milliseconds(20 + (id % 60));
    std::this_thread::sleep_for(latency);

    // Simulamos errores ocasionales (aprox. 1 de cada 15)
    if (id % 15 == 0)
    {
        throw std::runtime_error("HTTP error (simulated) for request id=" + std::to_string(id));
    }

    // Aquí iría el procesamiento real de la respuesta...
    // std::cout << "Request " << id << " OK\n";
}

// ============================================================
// main
// ============================================================
int main()
{
    // ---------------------------------------------------------
    // 1. Configuración e inyección de dependencias
    // ---------------------------------------------------------
    constexpr std::size_t NUM_THREADS = 6;          // Inyectable
    constexpr int         BATCH_SIZE  = 40;         // Peticiones por ciclo
    constexpr int         NUM_BATCHES = 4;          // Número de ciclos de polling

    auto exceptionHandler = std::make_shared<LoggingExceptionHandler>();

    // El número de threads y el handler se inyectan por constructor
    TaskEngine engine(NUM_THREADS, exceptionHandler);

    std::cout << "TaskEngine started with " << engine.GetThreadCount()
              << " worker threads\n\n";

    // ---------------------------------------------------------
    // 2. Loop de polling (simulación del sistema real)
    // ---------------------------------------------------------
    for (int batch = 0; batch < NUM_BATCHES; ++batch)
    {
        std::cout << ">>> Batch " << (batch + 1) << "/" << NUM_BATCHES
                  << " — enviando " << BATCH_SIZE << " tareas...\n";

        // Enviamos un batch de peticiones HTTP
        for (int i = 0; i < BATCH_SIZE; ++i)
        {
            const int requestId = batch * BATCH_SIZE + i;
            engine.Submit([requestId] {
                FetchData(requestId);
            });
        }

        // Esperamos a que termine el batch completo
        engine.WaitForIdle();

        // -----------------------------------------------------
        // 3. Consultamos métricas
        // -----------------------------------------------------
        const TaskMetrics m = engine.GetMetrics();

        std::cout << "--- Métricas después del batch " << (batch + 1) << " ---\n"
                  << "  Submitted      : " << m.submitted      << "\n"
                  << "  Completed      : " << m.completed      << "\n"
                  << "  Failed         : " << m.failed         << "\n"
                  << "  Avg Latency    : " << m.averageLatencyMs << " ms\n"
                  << "  Throughput     : " << m.tasksPerSecond << " tasks/s\n"
                  << "  Pending        : " << engine.GetPendingTaskCount() << "\n\n";

        // Pequeña pausa entre batches (simula intervalo de polling)
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    // ---------------------------------------------------------
    // 4. Parada limpia
    // ---------------------------------------------------------
    std::cout << "Stopping TaskEngine...\n";
    engine.Stop();

    // Métricas finales
    const TaskMetrics finalMetrics = engine.GetMetrics();
    std::cout << "\n=== Métricas finales ===\n"
              << "Total submitted : " << finalMetrics.submitted << "\n"
              << "Total completed : " << finalMetrics.completed << "\n"
              << "Total failed    : " << finalMetrics.failed << "\n"
              << "Avg latency     : " << finalMetrics.averageLatencyMs << " ms\n"
              << "Avg throughput  : " << finalMetrics.tasksPerSecond << " tasks/s\n";

    return 0;
}

// ============================================================
// Implementación de TaskEngine (sin PImpl)
// ============================================================
namespace taskEngine
{
    TaskEngine::TaskEngine(std::size_t numThreads,
                           std::shared_ptr<IExceptionHandler> exceptionHandler)
        : stopFlag(false)
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

    TaskEngine::~TaskEngine()
    {
        Stop();
    }

    void TaskEngine::Submit(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopFlag)
            {
                throw std::runtime_error("TaskEngine is stopped");
            }

            tasks.push(std::move(task));
            ++pendingTasks;
            ++metrics.submitted;
        }
        cv.notify_one();
    }

    void TaskEngine::WaitForIdle()
    {
        std::unique_lock<std::mutex> lock(mutex);
        idleCv.wait(lock, [this] {
            return pendingTasks == 0;
        });
    }

    void TaskEngine::Stop()
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopFlag)
            {
                return;
            }
            stopFlag = true;
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

    std::size_t TaskEngine::GetThreadCount() const
    {
        return workers.size();
    }

    std::size_t TaskEngine::GetPendingTaskCount() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return pendingTasks;
    }

    bool TaskEngine::IsRunning() const
    {
        return !stopFlag;
    }

    TaskMetrics TaskEngine::GetMetrics() const
    {
        std::lock_guard<std::mutex> lock(mutex);

        TaskMetrics m = metrics;

        if (m.completed > 0)
        {
            m.averageLatencyMs = m.totalLatencyMs / static_cast<double>(m.completed);
        }

        const auto now = std::chrono::steady_clock::now();
        const double elapsedSec = std::chrono::duration<double>(now - m.startTime).count();

        if (elapsedSec > 0.0)
        {
            m.tasksPerSecond = static_cast<double>(m.completed) / elapsedSec;
        }

        return m;
    }

    void TaskEngine::ResetMetrics()
    {
        std::lock_guard<std::mutex> lock(mutex);
        metrics = TaskMetrics{};
        metrics.startTime = std::chrono::steady_clock::now();
    }

    void TaskEngine::WorkerLoop()
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
                {
                    return;
                }

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
                {
                    idleCv.notify_all();
                }
            }
        }
    }

} // namespace taskEngine
