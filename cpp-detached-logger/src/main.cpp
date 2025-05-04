#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "DetachedLoggerApi.h"

using namespace detachedLogger;

int main() 
{
    DetachedLoggerResult setupResult = DetachedLoggerResult::WTF;

    setupResult = setUpDetachedLogger("theLog.log", 15);

    if (setupResult != DetachedLoggerResult::OK) 
    {
        std::cerr << "ERROR! Setting up logger: " << static_cast<int>(setupResult) << std::endl;

        return -1;
    }

    setupResult = initalizeDetachedLogger();

    if (setupResult != DetachedLoggerResult::OK) 
    {
        std::cerr << "ERROR! Initialize detached logger result: " << static_cast<int>(setupResult) << std::endl;

        return -1;
    }
    
    std::ostringstream ss;

    ss << "I am thread  ";
    ss << std::this_thread::get_id();

    for (int i = 0; i < 120; ++i)
    {
        LOG_INFO(ss.str() + ". Log msg " + std::to_string(i));

        if( i % 10 == 0) 
        {
            LOG_ERROR(ss.str() + ". Ups! " + std::to_string(i));
        }
    }

    // Simulate some work
    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (int i = 0; i < 53; ++i)
    {
        LOG_INFO(ss.str() + ". Log more msg " + std::to_string(i));
    }

    setupResult = finalizeDetachedLogger();

    if (setupResult != DetachedLoggerResult::OK) 
    {
        std::cerr << "ERROR! Finalize logger result: " << static_cast<int>(setupResult) << std::endl;

        return -1;
    }

    std::cout << "OK! Sample logger using detached logger finished successfully and detached log result: " << static_cast<int>(setupResult) << std::endl;

    return 0;
}