#ifndef DETACHED_LOGGER_API_H
#define DETACHED_LOGGER_API_H

#include <string>

namespace detachedLogger
{
    enum class DetachedLoggerResult
    {
        WTF = -1,
        OK = 0,
        ERROR = 1,
        ALEREADY_INITIALIZED = 2,
        ERROR_NOT_INITIALIZED = 3,
    };

    DetachedLoggerResult setUpDetachedLogger(const std::string& filename, size_t bufferSize);
    DetachedLoggerResult initalizeDetachedLogger();
    DetachedLoggerResult finalizeDetachedLogger();

    void log_info(const std::string& message);
    void log_error(const std::string& message);

    #define LOG_INFO(msg) log_info(msg);
    #define LOG_ERROR(msg) log_error(msg);
};

#endif // DETACHED_LOGGER_API_H