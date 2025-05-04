#include "DetachedLoggerApi.h"

#include <memory>
#include <string>

#include "IAsyncLogger.h"
#include "AsyncLogger.h"

namespace detachedLogger
{
    std::unique_ptr<IAsyncLogger> detachedLogger_;

    void log_info(const std::string& message) 
    {
        if (detachedLogger_ != nullptr) 
        {
            detachedLogger_->log("INFO: " + message);
        }
    }

    void log_error(const std::string& message) 
    {
        if (detachedLogger_ != nullptr) 
        {
            detachedLogger_->log("ERROR: " + message);
        }
    }

    DetachedLoggerResult setUpDetachedLogger(const std::string& filename, size_t bufferSize)
    {
        if (detachedLogger_ != nullptr) 
        {
            return DetachedLoggerResult::ALEREADY_INITIALIZED;
        }

        detachedLogger_ = std::make_unique<AsyncLogger>(filename, bufferSize);

        if (detachedLogger_ == nullptr) 
        {
            return DetachedLoggerResult::ERROR;
        }

        return DetachedLoggerResult::OK;
    }

    DetachedLoggerResult initalizeDetachedLogger()
    {
        if (detachedLogger_ == nullptr) 
        {
            return DetachedLoggerResult::ERROR_NOT_INITIALIZED;
        }

        detachedLogger_->initialize();

        return DetachedLoggerResult::OK;
    }

    DetachedLoggerResult finalizeDetachedLogger()
    {
        if (detachedLogger_ == nullptr) 
        {
            return DetachedLoggerResult::ERROR_NOT_INITIALIZED;
        }

        detachedLogger_->finalize();

        return DetachedLoggerResult::OK;
    }    
};