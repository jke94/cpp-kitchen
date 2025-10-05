# cpp-async-log

Asynchronous C++ logger implementation.

## Summary of Design

| #  | Category                        | Description                                                                                                                                                          | Status                       |
|----|---------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------------------|
| 1  | 🧠 **Thread Safety**             | The logger uses a `std::mutex` and `std::condition_variable` to safely coordinate access to the shared message queue between producer threads and the writer thread. | ✅ Implemented                |
| 2  | 🔄 **Batching Support**          | Messages are accumulated in batches (defined by `batch_size_`) and written together to minimize disk I/O operations.                                                 | ✅ Implemented                |
| 3  | ⚙️ **Constructor Parameter**     | `batchSize` is passed through the constructor to control batching behavior dynamically.                                                                              | ✅ Fixed (was ignored before) |
| 4  | 🧵 **Joinable Thread Model**     | The writer thread is launched joinable and properly synchronized via `join()` on shutdown — ensuring clean termination.                                              | ✅ Implemented                |
| 5  | 🚀 **Performance**               | Lock is released before disk I/O; batched writing reduces system calls and increases throughput.                                                                     | ✅ Optimized                  |
| 6  | 🧱 **Initialization Safety**     | Initialization is now protected by a mutex to avoid race conditions or double initialization.                                                                        | ✅ Hardened                   |
| 7  | 🧹 **Clean Finalization**        | `finalize()` gracefully stops the logger, flushes pending messages, and joins the writer thread.                                                                     | ✅ Implemented                |
| 8  | 🧽 **Destructor Safety**         | Destructor calls `finalize()` only if the logger is still running, preventing double-shutdown issues.                                                                | ✅ Improved                   |
| 9  | 💡 **Condition-based Wake-up**   | The writer thread sleeps efficiently using a conditional wait, waking only when new messages or shutdown events occur.                                               | ✅ Implemented                |
| 10 | 🪶 **I/O Separation**            | The writer loop releases the mutex before performing file I/O, ensuring minimal blocking for producers.                                                              | ✅ Best Practice              |
| 11 | ⚠️ **Error Handling**            | If the log file cannot be opened, an error is printed and logging stops safely.                                                                                      | ✅ Implemented                |
| 12 | 🧩 **Dependency Injection**      | The design follows an interface-based approach (`ILogger`) to support dependency injection and testing.                                                              | ✅ Implemented                |
| 13 | 🧠 **No Busy Waiting**           | Uses condition variables instead of polling or sleeping; no CPU waste when idle.                                                                                     | ✅ Optimized                  |
| 14 | 🔒 **Atomic State**              | `std::atomic<bool> running_` ensures thread-safe lifecycle control without data races.                                                                               | ✅ Implemented                |
| 15 | 🧾 **Flush Policy**              | Each batch is flushed immediately to ensure data persistence while maintaining performance.                                                                          | ✅ Implemented                |
| 16 | 🧱 **Enum Result Type**          | Added `LogResult` enum to represent operation outcomes (success, error, etc.).                                                                                       | ✅ Implemented                |
| 17 | ⚙️ **init() Return Value**       | `init()` now returns a `LogResult` indicating success, already initialized, or file open failure.                                                                    | ✅ Added                      |
| 18 | 🧹 **finalize() Return Value**   | `finalize()` now returns a `LogResult` indicating success or not initialized.                                                                                        | ✅ Added                      |
| 19 | 🧠 **Pre-check File Access**     | Before starting writer thread, `init()` validates file can be opened.                                                                                                | ✅ Added                      |
| 20 | 🧩 **Error Propagation**         | Main function now checks `LogResult` values for initialization and finalization.                                                                                     | ✅ Implemented                |
| 21 | 💡 **Thread Safety Maintained**  | Mutex and condition variable usage remain unchanged and correct.                                                                                                    | ✅ Verified                   |

## ✅ Example Usage

Below is a minimal example demonstrating how to initialize, use, and finalize the asynchronous logger
while checking return values from `init()` and `finalize()` using the `LogResult` enum.

```cpp
#include "Logger.h"
#include <iostream>
#include <memory>

using namespace asynclog;

int main()
{
    // Create logger via dependency injection using the interface
    std::shared_ptr<ILogger> logger = std::make_shared<Logger>(50); // Batch size: 50 messages

    // Initialize logger and check result
    LogResult initResult = logger->init("app.log");
    if (initResult != LogResult::Success)
    {
        std::cerr << "[Main] Logger initialization failed with code: "
                  << static_cast<int>(initResult) << std::endl;
        return -1;
    }

    // Simulate concurrent logging from multiple threads
    std::thread t1([&]() {
        for (int i = 0; i < 10; ++i)
            logger->log("[T1] Message #" + std::to_string(i));
    });

    std::thread t2([&]() {
        for (int i = 0; i < 10; ++i)
            logger->log("[T2] Message #" + std::to_string(i));
    });

    // Wait for threads
    t1.join();
    t2.join();

    // Finalize logger and check result
    LogResult finalizeResult = logger->finalize();
    if (finalizeResult != LogResult::Success)
    {
        std::cerr << "[Main] Logger finalization failed with code: "
                  << static_cast<int>(finalizeResult) << std::endl;
        return -2;
    }

    std::cout << "✅ All messages logged and file closed successfully." << std::endl;
    return 0;
}
```
## 🧾 Return Code Reference

The `LogResult` enumeration defines the possible outcomes for logger operations such as `init()` and `finalize()`.  
This helps developers handle errors explicitly and make runtime behavior more predictable.

| Enum Value                  | Numeric Value | Description                                                                 |
|-----------------------------|----------------|-----------------------------------------------------------------------------|
| ⚙️ `LogResult::Success`      | `0`            | Operation completed successfully.                                           |
| 🚫 `LogResult::FileOpenError`| `2`            | Failed to open the specified log file.                                      |
| ⚠️ `LogResult::NotInitialized`| `3`           | Operation attempted before logger was initialized.                          |
| 🔁 `LogResult::AlreadyInitialized` | `4`      | Attempted to re-initialize an already running logger instance.              |
| 💥 `LogResult::WTF`          | `-1`           | Unexpected or undefined behavior occurred (should not happen in practice).  |

### 💡 Usage Tip
You can safely check results by comparing against `LogResult::Success`,  
or by casting the enum to `int` for debug output:

```cpp
LogResult result = logger->init("app.log");
if (result != LogResult::Success) {
    std::cerr << "Logger init failed, code = " << static_cast<int>(result) << std::endl;
}
