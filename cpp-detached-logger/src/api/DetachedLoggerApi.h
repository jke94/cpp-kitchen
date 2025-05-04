#ifndef DETACHED_LOGGER_API_H
#define DETACHED_LOGGER_API_H

#include <string>

namespace detachedLogger
{
    /**
     * @brief Enum class to represent the result of the DetachedLogger operations.
     */
    enum class DetachedLoggerResult
    {
        WTF = -1,
        OK = 0,
        ERROR = 1,
        ALEREADY_INITIALIZED = 2,
        ERROR_NOT_INITIALIZED = 3,
    };

    /**
     * @brief Sets up the detached logger with the specified filename and buffer size.
     * @param filename The name of the file to log to.
     * @param bufferSize The size of the buffer to use for logging.
     * @return DetachedLoggerResult indicating the result of the operation.
     */
    DetachedLoggerResult setUpDetachedLogger(const std::string& filename, size_t bufferSize);

    /**
     * @brief Initializes the detached logger.
     * @return DetachedLoggerResult indicating the result of the operation.
     */
    DetachedLoggerResult initalizeDetachedLogger();

    /**
     * @brief Finalizes the detached logger.
     * @return DetachedLoggerResult indicating the result of the operation.
     */
    DetachedLoggerResult finalizeDetachedLogger();

    /**
     * @brief Function to log messages with information level.
     * @param message The message to log.
     */
    void log_info(const std::string& message);

    /**
     * @brief Function to log messages with error level.
     * @param message The message to log.
     */
    void log_error(const std::string& message);

    /**
     * @brief Function to log messages with debug level called by the user to log messages.
     * @param message The message to log.
     */
    #define LOG_INFO(msg) log_info(msg);

    /**
     * @brief Function to log messages with error level called by the user to log messages.
     * @param message The message to log.
     */
    #define LOG_ERROR(msg) log_error(msg);
};

#endif // DETACHED_LOGGER_API_H