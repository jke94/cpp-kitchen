#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace asynclog
{
    /**
     * @brief Log result enumeration for operation status.
     */
    enum class LogResult
    {
        WTF = -1,
        Success,
        FileOpenError,
        NotInitialized,
        AlreadyInitialized
    };

    /**
     * @brief Abstract logger interface
     */
    class ILogger 
    {
    public:
        virtual ~ILogger() {}

        /**
         * @brief Logger initialization with a filename.
         */
        virtual LogResult init(const std::string& filename) = 0;

        /**
         * @brief Logs a message.
         * @param message The message to log.
         */
        virtual void log(const std::string& message) = 0;

        /**
         * @brief Finalizes the logger, ensuring all messages are processed.
         */
        virtual LogResult finalize() = 0;

    protected:
        ILogger() = default;
        ILogger(const ILogger&) = default;
        ILogger& operator=(const ILogger&) = default;

    };

} // namespace asynclog

namespace asynclog
{
    /**
     * @brief Asynchronous logger implementation with joinable thread,
     * thread-safe buffer and batch writing.
     */
    class Logger final : public ILogger 
    {
    public:
        explicit Logger(size_t batchSize);
        ~Logger() override;

        LogResult init(const std::string& filename) override;
        void log(const std::string& message) override;
        LogResult finalize() override;

    private:
        void writerLoop();

        std::string filename_;
        std::queue<std::string> buffer_;
        std::mutex mutex_;
        std::condition_variable cond_var_;
        std::atomic<bool> running_;
        std::thread writer_thread_;
        size_t batch_size_;
    };

} // namespace asynclog

using namespace asynclog;

int main()
{
    std::shared_ptr<ILogger> logger = std::make_shared<Logger>(50);

    const LogResult initResult = logger->init("app.log");

    if (initResult != LogResult::Success)
    {
        std::cerr << "Logger initialization failed! Return code: " << static_cast<int>(initResult) << std::endl;
        return -1;
    }

    // Thread configuration
    std::map<std::string, int> threadConfig = {
        {"T1", 6}, 
        {"T2", 87}, 
        {"T3", 12}, 
        {"T4", 5},
        {"T5", 17}, 
        {"T6", 2}, 
        {"T7", 11}, 
        {"T8", 30},
        {"T9", 67}, 
        {"T10", 5}, 
        {"T11", 9}, 
        {"T12", 15}
    };

    std::vector<std::thread> threads;
    threads.reserve(threadConfig.size());

    // Launch threads
    for (const auto& entry : threadConfig)
    {
        const std::string& threadName = entry.first;
        int messageCount = entry.second;

        threads.emplace_back([logger, threadName, messageCount]()
        {
            for (int i = 0; i < messageCount; ++i)
            {
                logger->log("[" + threadName + "] Message #" + std::to_string(i));
            }
        });
    }

    // Wait for all threads
    for (auto& t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
        
    const LogResult finalizeResult = logger->finalize();

    if (finalizeResult != LogResult::Success)
    {
        std::cerr << "Logger finalization failed! Return code: " << static_cast<int>(finalizeResult) << std::endl;
        return -1;
    }

    std::cout << "All messages have been logged successfully." << std::endl;
    return 0;
}

namespace asynclog
{
    Logger::Logger(size_t batchSize)
        : running_(false), batch_size_(batchSize)
    {
    }

    Logger::~Logger()
    {
        if (running_)
        {
            finalize();
        }
    }

    LogResult Logger::init(const std::string& filename)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (running_)
        {
            std::cerr << "[Logger] Already initialized: " << filename_ << std::endl;
            return LogResult::AlreadyInitialized;
        }

        filename_ = filename;

        // Check if file can be opened before starting the thread
        std::ofstream test(filename_.c_str(), std::ios::app);
        if (!test.is_open())
        {
            std::cerr << "[Logger] Could not open file: " << filename_ << std::endl;
            return LogResult::FileOpenError;
        }

        running_ = true;
        writer_thread_ = std::thread(&Logger::writerLoop, this);

        return LogResult::Success;
    }

    void Logger::log(const std::string& message)
    {
        if (!running_)
        {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_.push(message);
        }

        cond_var_.notify_one();
    }

    LogResult Logger::finalize()
    {
        if (!running_)
        {
            return LogResult::NotInitialized;
        }

        {
            std::lock_guard<std::mutex> lock(mutex_);
            running_ = false;
        }

        cond_var_.notify_one();

        if (writer_thread_.joinable())
        {
            writer_thread_.join();
        }

        return LogResult::Success;
    }

    void Logger::writerLoop()
    {
        std::ofstream file(filename_.c_str(), std::ios::app);
        if (!file.is_open())
        {
            std::cerr << "[Logger] Writer thread failed to open file: " << filename_ << std::endl;
            running_ = false;
            return;
        }

        while (true)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_var_.wait(lock, [this]()
            {
                return !buffer_.empty() || !running_;
            });

            std::string batch;
            size_t batchCount = 0;

            while (!buffer_.empty() && batchCount < batch_size_)
            {
                batch += buffer_.front();
                batch += '\n';
                buffer_.pop();
                ++batchCount;
            }

            lock.unlock();

            if (!batch.empty())
            {
                file << batch;
                file.flush();
            }

            if (!running_ && buffer_.empty())
            {
                break;
            }
        }
    }

} // namespace asynclog
