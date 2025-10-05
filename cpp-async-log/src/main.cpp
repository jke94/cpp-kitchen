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
     * @brief Abstract logger interface
     */
    class ILogger 
    {
    public:
        virtual ~ILogger() {}

        /**
         * @brief Logger initialization with a filename.
         */
        virtual void init(const std::string& filename) = 0;

        /**
         * @brief Logs a message.
         * @param message The message to log.
         */
        virtual void log(const std::string& message) = 0;

        /**
         * @brief Finalizes the logger, ensuring all messages are processed.
         */
        virtual void finalize() = 0;

    protected:
        ILogger() = default;
        ILogger(const ILogger&) = default;
        ILogger& operator=(const ILogger&) = default;

    };

} // namespace asynclog

namespace asynclog
{
    /**
     * @brief Asynchronous logger implementation with joinable thread and thread-safe buffer and batch writing.
     */
    class Logger final : public ILogger 
    {
    public:
        Logger(size_t batchSize);
        ~Logger() override;

        void init(const std::string& filename) override;
        void log(const std::string& message) override;
        void finalize() override;

    private:

        /**
         * @brief Writer thread function that processes log messages from the buffer.
         */
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
    // Dependency injection via interface.
    std::shared_ptr<ILogger> logger = std::make_shared<Logger>(50); // Batch size of 50 messages.
    logger->init("app.log");

    // Number of threads and messages per thread definition.
    // Key: thread name, Value: number of messages
    std::map<std::string, int> threadConfig = {
        {"T1", 5},
        {"T2", 1},
        {"T3", 6},
        {"T4", 7},
        {"T5", 1},
        {"T6", 2},
        {"T7", 1},
        {"T8", 3},
        {"T9", 6},
        {"T10", 5}
    };

    std::vector<std::thread> threads;
    threads.reserve(threadConfig.size());

    // Threads launch according to configuration
    for (const auto& entry : threadConfig)
    {
        const std::string& threadName = entry.first;
        int messageCount = entry.second;

        threads.emplace_back([logger, threadName, messageCount]()
        {
            for (int i = 0; i < messageCount; ++i)
            {
                logger->log("[" + threadName + "] Mensaje número " + std::to_string(i));
            }
        });
    }

    // Wait for all threads to finish
    for (auto& t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    // Finalize the logger to ensure all messages are flushed.
    logger->finalize();

    std::cout << "All messages have been logged successfully." << std::endl;

    return 0;
}

namespace asynclog
{
    Logger::Logger(size_t batchSize) 
        : running_(false),
        batch_size_(batchSize) // Default batch size to 100 messages.
    {

    }

    Logger::~Logger() 
    {
        if (running_)
        {
            // NOTE: Close the logger if the user forgot to do it.
            finalize();
        } 
    }

    void Logger::init(const std::string& filename) 
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(running_)
        {
            // Avoid re-initialization if already running.
            std::cerr << "[Logger] Already initialized with file: " << filename_ << std::endl;
            return;
        }

        filename_ = filename;
        running_ = true;

        // Runs the writer thread.
        writer_thread_ = std::thread(&Logger::writerLoop, this);
    }

    void Logger::log(const std::string& message) 
    {
        if(!running_)
        {
            // Ignore if not running.
            return;
        }

        {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_.push(message);
        }

        // Notify the writer thread that a new message is available.
        cond_var_.notify_one();
    }

    void Logger::finalize() 
    {
        if(!running_)
        {
            // Ignore if not running.
            return;
        }

        {
            std::lock_guard<std::mutex> lock(mutex_);
            running_ = false;
        }
        cond_var_.notify_one();

        // Wait for the writer thread to finish.
        if (writer_thread_.joinable()) 
        {
            writer_thread_.join();
        }
    }

    void Logger::writerLoop() 
    {
        std::ofstream file(filename_.c_str(), std::ios::app);
        if (!file.is_open()) 
        {
            std::cerr << "The log file could not be opened: " << filename_ << std::endl;
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

            // Extrae mensajes del buffer (bloque protegido)
            while (!buffer_.empty() && batchCount < batch_size_) 
            {
                batch += buffer_.front();
                batch += '\n';
                buffer_.pop();
                ++batchCount;
            }

            lock.unlock(); // liberamos el lock antes del I/O

            // Escritura agrupada
            if (!batch.empty()) 
            {
                file << batch;
                file.flush(); // fuerza escritura inmediata
            }

            // Finalizar si se solicitó detener y ya no hay mensajes
            if (!running_ && buffer_.empty()) 
            {
                break;
            }
        }
    }

} // namespace asynclog
