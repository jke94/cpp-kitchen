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
 * @brief Engine API definition
 * 
 * It contains only the minimal classes and interfaces that a client needs to consume the API.
 */
namespace taskEngineApi
{
    /**
     * @brief Interface for handling exceptions thrown by tasks.
     * @note OnException must be noexcept and thread-safe. 
     * If it throws, the behavior is undefined (may call std::terminate).
     */
    class IExceptionHandler
    {
    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~IExceptionHandler() = default;

        /**
         * @brief Called when a task throws an exception.
         * @param eptr The exception pointer to the thrown exception.
         * @param context A string providing context about where the exception occurred.
         * @note This method must be noexcept and thread-safe.
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
     * @brief Default exception handler that does nothing.
     */
    class NullExceptionHandler final : public IExceptionHandler
    {
    public:
        void OnException(
            std::exception_ptr eptr,
            const std::string& context
        ) noexcept override;
    };

    /**
     * @brief Struct for task metrics (submitted, completed, failed, latency, throughput).
     */
    struct TaskMetrics
    {
        // Submitted tasks.
        std::uint64_t submitted = 0;
        
        // Completed tasks.
        std::uint64_t completed = 0;
        
        // Failed tasks with exceptions.
        std::uint64_t failed = 0;

        // Latency metrics (in milliseconds).
        double totalLatencyMs  = 0.0;

        // Average latency and throughput (computed on demand).
        double averageLatencyMs = 0.0;

        // Throughput metrics (tasks per second).
        double tasksPerSecond   = 0.0;

        // Start time for throughput calculation.
        std::chrono::steady_clock::time_point startTime;
    };

    /**
     * @brief Interface for the task engine.
     */
    class IEngine
    {
    public:
        virtual ~IEngine() = default;

        /**
         * @brief Submits a task to the engine.
         * @param task A std::function representing the task to be executed.
         * @throws std::invalid_argument if the task is empty.
         * @throws std::runtime_error if the engine is stopped.
         */
        virtual void Submit(std::function<void()> task) = 0;

        template <typename F, typename... Args>
        auto SubmitWithResult(F&& f, Args&&... args)
            -> std::future<std::invoke_result_t<std::decay_t<F>, Args...>>;

        virtual void WaitForIdle() = 0;
        virtual void Stop() = 0;

        virtual std::size_t GetThreadCount() const = 0;
        virtual std::size_t GetPendingTaskCount() const = 0;

        /**
         * Snapshot del estado. NO es seguro usarlo como:
         *   if (IsRunning()) { Submit(...); }
         * Existe condición de carrera (TOCTOU) con Stop().
         * Submit() ya valida internamente y lanza si está detenido.
         */
        virtual bool IsRunning() const = 0;

        // ----- APIs de métricas -----
        virtual TaskMetrics GetMetrics() const = 0;

        /**
         * Reinicia contadores y reloj.
         * Si se llama mientras hay tareas en vuelo, las estadísticas
         * posteriores pueden mezclar datos de antes y después del reset
         * (best-effort). Para métricas consistentes, llamar solo en idle.
         */
        virtual void ResetMetrics() = 0;

    protected:
        IEngine() = default;
        IEngine(const IEngine&) = default;
        IEngine& operator=(const IEngine&) = default;
    };

    /**
     * @brief Base implementation of the task engine.
     */
    class Engine final : public IEngine
    {
    public:
        /**
         * @param numThreads         Número de worker threads (> 0)
         * @param exceptionHandler   Handler de excepciones (obligatorio, DI)
         */
        explicit Engine(
            std::size_t numThreads,
            std::shared_ptr<IExceptionHandler> exceptionHandler
        );

        ~Engine() override;

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

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

        // Número de threads inmutable tras la construcción.
        // Evita data race en GetThreadCount() vs Stop().
        const std::size_t                   threadCount;

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
    auto IEngine::SubmitWithResult(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<std::decay_t<F>, Args...>>
    {
        using ReturnType = std::invoke_result_t<std::decay_t<F>, Args...>;

        // Evitamos std::bind (problemas con move-only y copias innecesarias).
        // Usamos lambda + perfect forwarding + packaged_task.
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            [fn = std::decay_t<F>(std::forward<F>(f)),
             tup = std::make_tuple(std::forward<Args>(args)...)]() mutable
            {
                return std::apply(std::move(fn), std::move(tup));
            }
        );

        std::future<ReturnType> future = task->get_future();

        Submit([task]() {
            (*task)();
        });

        return future;
    }

} // namespace taskEngineApi

namespace taskEngineClient
{
    /**
     * @brief Latency simulation minimum (milliseconds).
     */
    const int MIN_LATENCY_MS = 80;

    /**
     * @brief Latency simulation maximum (milliseconds).
     */
    const int MAX_LATENCY_MS = 120;

    /**
     * @brief Percentage of requests that will fail (0–100).
     * Example: 7 → aproximadamente 7 % de fallos.
     */
    const int FAILURE_PERCENTAGE = 7;

    /**
     * @brief HTTP request simulation (blocking, with variable latency and configurable failure rate).
     * @note latency uniform in [MIN_LATENCY_MS, MAX_LATENCY_MS] and failure rate approximately FAILURE_PERCENTAGE % of requests (deterministic by id for reproducibility in tests).
     */
    void fetchData(int id);

    /**
     * @brief Handler exception manager for client code.
     */
    class LoggingExceptionHandler final : public taskEngineApi::IExceptionHandler
    {
    public:
        void OnException(
            std::exception_ptr eptr,
            const std::string& context
        ) noexcept override;
    };

} // namespace taskEngineClient

using namespace taskEngineApi;
using namespace taskEngineClient;

/**
 * @brief Program entrypoint to simulate consume of TaskEngine API.
 */
int main(int argc, char* argv[])
{
    // ---------------------------------------------------------
    // 1. Configuración e inyección de dependencias
    // ---------------------------------------------------------
    constexpr std::size_t NUM_THREADS = 6;          // Inyectable
    constexpr int         BATCH_SIZE  = 40;         // Peticiones por ciclo
    constexpr int         NUM_BATCHES = 4;          // Número de ciclos de polling

    auto exceptionHandler = std::make_shared<LoggingExceptionHandler>();

    // El número de threads y el handler se inyectan por constructor
    // (sin parámetros por defecto: el cliente debe decidir explícitamente)
    Engine engine(NUM_THREADS, exceptionHandler);

    std::cout << "TaskEngine started with " << engine.GetThreadCount()
              << " worker threads\n"
              << "Failure simulation: " << FAILURE_PERCENTAGE << "%\n"
              << "Latency range: [" << MIN_LATENCY_MS << "–" << MAX_LATENCY_MS << "] ms\n\n";

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
            engine.Submit([requestId]
            {
                fetchData(requestId);
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
                  << "  Failed         : " << m.failed
                  << " (aprox. " << (m.failed * 100.0 / m.submitted) << "%)\n"
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
              << "Total failed    : " << finalMetrics.failed
              << " (aprox. " << (finalMetrics.failed * 100.0 / finalMetrics.submitted) << "%)\n"
              << "Avg latency     : " << finalMetrics.averageLatencyMs << " ms\n"
              << "Avg throughput  : " << finalMetrics.tasksPerSecond << " tasks/s\n";

    return 0;
}

/**
 * @brief Implementation of the TaskEngine API.
 */
namespace taskEngineApi
{
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

    void Engine::Submit(std::function<void()> task)
    {
        // Defensive validation: empty tasks are not allowed
        if (!task)
        {
            throw std::invalid_argument("TaskEngine::Submit: empty task is not allowed");
        }

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

    std::size_t Engine::GetThreadCount() const
    {
        // Inmutable → sin data race con Stop()
        return threadCount;
    }

    std::size_t Engine::GetPendingTaskCount() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return pendingTasks;
    }

    bool Engine::IsRunning() const
    {
        // Snapshot. Ver documentación de la interface.
        return !stopFlag.load(std::memory_order_acquire);
    }

    TaskMetrics Engine::GetMetrics() const
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

                // Defensa en profundidad: aunque OnException es noexcept,
                // un handler mal implementado podría lanzar y matar el proceso.
                // Aislamos el worker completamente.
                try
                {
                    exceptionHandler->OnException(
                        std::current_exception(),
                        "TaskEngine::Worker"
                    );
                }
                catch (...)
                {
                    // Handler violó el contrato noexcept.
                    // No re-lanzamos: el worker debe seguir vivo.
                    // En producción se podría loguear a stderr de forma mínima.
                }
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

    void NullExceptionHandler::OnException(
        std::exception_ptr /*eptr*/,
        const std::string& context
    ) noexcept
    {
        std::cerr << "[NullExceptionHandler] Exception in context: " << context << std::endl;
    }

} // namespace taskEngineApi

namespace taskEngineClient
{
    void fetchData(int id)
    {
        // Defensive validation of constants (compile-time)
        static_assert(MIN_LATENCY_MS >= 0, "MIN_LATENCY_MS must be >= 0");
        static_assert(MAX_LATENCY_MS >= MIN_LATENCY_MS, "MAX_LATENCY_MS must be >= MIN_LATENCY_MS");
        static_assert(FAILURE_PERCENTAGE >= 0 && FAILURE_PERCENTAGE <= 100,
                      "FAILURE_PERCENTAGE must be in [0, 100]");

        // Simulated latency: deterministically based on id to ensure reproducibility in tests.
        const int range = MAX_LATENCY_MS - MIN_LATENCY_MS + 1;
        const auto latency = std::chrono::milliseconds(MIN_LATENCY_MS + (id % range));
        std::this_thread::sleep_for(latency);

        // Simulate failure based on the configured percentage. 
        // Using modulo 100 to get a deterministic "percentile" based on the id.
        if (FAILURE_PERCENTAGE > 0 && (id % 100) < FAILURE_PERCENTAGE)
        {
            throw std::runtime_error(
                "HTTP error (simulated) for request id=" + std::to_string(id)
            );
        }

        // Processing successful request (for demonstration purposes, we just print a message)
        // std::cout << "Request " << id << " OK\n";
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
