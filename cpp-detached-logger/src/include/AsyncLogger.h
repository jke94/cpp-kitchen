#ifndef ASYNC_LOGGER_H
#define ASYNC_LOGGER_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <string>
#include <vector>

#include "IAsyncLogger.h"

namespace detachedLogger
{
    class AsyncLogger final : public IAsyncLogger
    {
    public:
        AsyncLogger(const std::string& filename, size_t bufferSize);
        ~AsyncLogger();
        void initialize() override;
        void finalize() override;
        void log(const std::string& message) override;

    private:
        void worker();
        void flushToFile(const std::vector<std::string>& messages);    
        void flushRemaining();
    
        std::string filename_;
        size_t bufferSize_;
        std::vector<std::string> buffer_;
        std::mutex mutex_;
        std::condition_variable condVar_;
        std::atomic<bool> exitFlag_;
        std::thread workerThread_;
        bool initialized_;
    };
    
}; // namespace detachedLogger

#endif // ASYNC_LOGGER_H