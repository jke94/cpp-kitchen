/**
 * Ejemplo de uso de TaskEngine
 * ---------------------------
 * Simula un sistema de polling HTTP concurrente.
 * Demuestra:
 *  - Inyección de dependencias (nº de threads + IExceptionHandler)
 *  - Envío de tareas
 *  - Métricas (throughput, latencia media, etc.)
 *  - Manejo de excepciones
 */

#include "TaskEngine.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdexcept>
#include <atomic>

// ============================================================
// Handler de excepciones de ejemplo
// ============================================================
class LoggingExceptionHandler : public IExceptionHandler
{
public:
    void OnException(std::exception_ptr eptr, const std::string& context) noexcept override
    {
        try
        {
            if (eptr)
                std::rethrow_exception(eptr);
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
