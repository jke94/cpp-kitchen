#include <iostream>
#include <string>
#include <thread>

#include "DetachedLoggerApi.h"

using namespace detachedLogger;

int main() 
{
    DetachedLoggerResult setupResult = setUpDetachedLogger("log.txt", 5);

    if (setupResult != DetachedLoggerResult::OK) 
    {
        std::cerr << "Error setting up logger: " << static_cast<int>(setupResult) << std::endl;
        return -1;
    }

    DetachedLoggerResult initResult = initalizeDetachedLogger();

    if (initResult != DetachedLoggerResult::OK) 
    {
        std::cerr << "Error initializing logger: " << static_cast<int>(initResult) << std::endl;
        return -1;
    }
    
    for (int i = 0; i < 120; ++i) 
    {
        LOG_INFO("Log msg " + std::to_string(i));

        if( i % 10 == 0) 
        {
            LOG_ERROR("Ups! " + std::to_string(i));
        }
    }

    // Simulate some work
    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < 50; ++i) 
    {
        LOG_INFO("Log more msg " + std::to_string(i));
    }

    DetachedLoggerResult finalizeResult = finalizeDetachedLogger();

    if (finalizeResult != DetachedLoggerResult::OK) 
    {
        std::cerr << "Error finalizing logger: " << static_cast<int>(finalizeResult) << std::endl;
    }

    return 0;
}