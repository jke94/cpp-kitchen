#ifndef TASK_ENGINE_H
#define TASK_ENGINE_H

#include <functional>
#include <future>
#include <memory>
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <string>

// ============================================================
// Exception Handler (inyectable)
// ============================================================

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
    virtual void OnException(std::exception_ptr eptr,
                             const std::string& context = "") noexcept = 0;
};

/**
 * Implementación por defecto (no hace nada).
 * Útil cuando no se quiere inyectar un handler real.
 */
class NullExceptionHandler final : public IExceptionHandler
{
public:
    void OnException(std::exception_ptr, const std::string& = "") noexcept override {}
};

// ============================================================
// Métricas
// ============================================================

struct TaskMetrics
{
    std::uint64_t submitted      = 0;   // Tareas enviadas
    std::uint64_t completed      = 0;   // Tareas finalizadas con éxito
    std::uint64_t failed         = 0;   // Tareas que lanzaron excepción
    double        totalLatencyMs = 0.0; // Suma de latencias (para media)
    double        averageLatencyMs = 0.0;
    double        tasksPerSecond = 0.0;
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
// Implementación concreta
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
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

// -----------------------------------------------------------------
// Template implementation (debe estar en el header)
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

    Submit([task]() { (*task)(); });

    return future;
}

#endif // TASK_ENGINE_H