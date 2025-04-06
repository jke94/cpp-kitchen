#ifndef I_LOGGER_H
#define I_LOGGER_H

#include <string>

struct ILogger 
{
    virtual void log(const std::string& msg) = 0;
    virtual ~ILogger() = default;
};

#endif // I_LOGGER_H