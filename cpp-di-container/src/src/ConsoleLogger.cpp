#include <string>
#include <iostream>

#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger(std::string filename) 
{
    filename_ = filename;
}

void ConsoleLogger::log(const std::string& msg) 
{
    std::cout << this <<" | ConsoleLogger: " << msg << ". Log file name: " << filename_ << std::endl;
}