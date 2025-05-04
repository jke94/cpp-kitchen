#include "AsyncLogger.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

namespace detachedLogger
{
    AsyncLogger::AsyncLogger(
        const std::string& filename, 
        size_t bufferSize
    ) : 
        filename_(filename), 
        bufferSize_(bufferSize), 
        exitFlag_(false), 
        initialized_(false)
    {
        
    }

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
        workerThread_ = std::thread(
            [this]()
            { 
                this->worker(); 
            }
        );

        initialized_ = true;

        std::ostringstream ss;

        ss << "Thread ";
        ss << std::this_thread::get_id();
        ss << ". AsyncLogger initialized. Buffer size: ";
        ss << bufferSize_;
        ss << " and filename: ";
        ss << filename_;

        log(ss.str());
    }

    void AsyncLogger::finalize()
    {
        std::ostringstream ss;

        ss << "Thread ";
        ss << std::this_thread::get_id();
        ss << ". AsyncLogger finalize.";

        log(ss.str());

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
} // namespace detachedLogger