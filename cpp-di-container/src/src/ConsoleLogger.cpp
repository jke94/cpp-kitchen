#include "ConsoleLogger.h"

#include <string>
#include <iostream>

ConsoleLogger::ConsoleLogger(std::string filename) 
{
    filename_ = filename;
}

void ConsoleLogger::log(const std::string& msg) 
{
    std::cout << this <<" | ConsoleLogger: " << msg << ". Log file name: " << filename_ << std::endl;
}