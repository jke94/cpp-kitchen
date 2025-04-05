#include <string>
#include <iostream>

#include "ConsoleLogger.h"

void ConsoleLogger::log(const std::string& msg) 
{
    std::cout << this <<" | ConsoleLogger: " << msg << std::endl;
}