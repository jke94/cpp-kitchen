#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <iostream>
#include <string>
#include "ILogger.h"

class ConsoleLogger : public ILogger 
{
private:
    std::string filename_;    

    public:
        ConsoleLogger(std::string filename);
        void log(const std::string& msg) override;
    };

#endif // CONSOLE_LOGGER_H