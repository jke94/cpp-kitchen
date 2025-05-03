#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <string>
#include <thread>

namespace detachedLogger
{
    class IAsyncLogger 
    {
    public:
        virtual ~IAsyncLogger() = default;
        virtual void initialize() = 0;
        virtual void finalize() = 0;
        virtual void log(const std::string& message) = 0;    
    
    protected:
        IAsyncLogger() {}
        IAsyncLogger(const IAsyncLogger&) = delete;
        IAsyncLogger& operator=(const IAsyncLogger&) = delete;
        IAsyncLogger(IAsyncLogger&&) = delete;
        IAsyncLogger& operator=(IAsyncLogger&&) = delete;
    };
    
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

    IAsyncLogger* createAsyncLogger(const std::string& filename, size_t bufferSize);

    AsyncLogger::AsyncLogger(
        const std::string& filename, 
        size_t bufferSize
    ) : 
        filename_(filename), 
        bufferSize_(10), 
        exitFlag_(false), 
        initialized_(false)
    {}

    AsyncLogger::~AsyncLogger() 
    {
        finalize();
    }

    void AsyncLogger::initialize()
    {
        if (initialized_)
        {
            return;
        } 

        exitFlag_ = false;
        workerThread_ = std::thread([this]() { this->worker(); });
        initialized_ = true;
    }

    void AsyncLogger::finalize()
    {
        if (!initialized_)
        {
            return;
        }

        {
            std::unique_lock<std::mutex> lock(mutex_);
            exitFlag_ = true;
            condVar_.notify_all();
        }

        if (workerThread_.joinable()) 
        {
            workerThread_.join();
        }

        initialized_ = false;
    }

    void AsyncLogger::log(const std::string& message)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        buffer_.push_back(message);

        if (buffer_.size() >= bufferSize_) 
        {
            condVar_.notify_all();
        }
    }

    void AsyncLogger::worker() 
    {
        while (true) 
        {
            std::vector<std::string> localBuffer;

            {
                std::unique_lock<std::mutex> lock(mutex_);
                condVar_.wait(lock, [this] {
                    return buffer_.size() >= bufferSize_ || exitFlag_;
                });

                if (exitFlag_ && buffer_.empty()) break;

                buffer_.swap(localBuffer);
            }

            flushToFile(localBuffer);
        }

        // Volcado final si quedan mensajes
        flushRemaining();
    }

    void AsyncLogger::flushToFile(const std::vector<std::string>& messages)
    {
        std::ofstream outFile(filename_, std::ios::app);

        for (const auto& msg : messages) 
        {
            outFile << msg << std::endl;
        }
    }

    void AsyncLogger::flushRemaining()
    {
        std::vector<std::string> remaining;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_.swap(remaining);
        }

        flushToFile(remaining);
    }

    IAsyncLogger* createAsyncLogger(const std::string& filename, size_t bufferSize)
    {
        return new AsyncLogger(filename, bufferSize);
    }
};

using namespace detachedLogger;

int main() 
{
    std::unique_ptr<IAsyncLogger> logger = std::unique_ptr<IAsyncLogger>(createAsyncLogger("log.txt", 5));

    logger->initialize();

    for (int i = 0; i < 120; ++i) 
    {
        logger->log("Log msg " + std::to_string(i));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < 50; ++i) 
    {
        logger->log("Log more msg " + std::to_string(i));
    }

    logger->finalize();

    return 0;
}