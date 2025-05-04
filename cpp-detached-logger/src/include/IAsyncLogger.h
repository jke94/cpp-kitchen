#ifndef I_ASYNC_LOGGER_H
#define I_ASYNC_LOGGER_H

#include <string>

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
        IAsyncLogger() = default;
        IAsyncLogger(const IAsyncLogger&) = delete;
        IAsyncLogger& operator=(const IAsyncLogger&) = delete;
        IAsyncLogger(IAsyncLogger&&) = delete;
        IAsyncLogger& operator=(IAsyncLogger&&) = delete;
    };
    
}; // namespace detachedLogger


#endif // I_ASYNC_LOGGER_H