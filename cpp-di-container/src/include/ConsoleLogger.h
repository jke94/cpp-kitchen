#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <iostream>
#include "ILogger.h"

class ConsoleLogger : public ILogger 
{
    public:
        void log(const std::string& msg) override 
        {
            std::cout << this <<" | ConsoleLogger: " << msg << std::endl;
        }
    };

#endif // CONSOLE_LOGGER_H