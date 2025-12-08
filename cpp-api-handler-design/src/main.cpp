#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "PublicApi.h"

static std::mutex g_cv_m;
static std::condition_variable g_cv;
static bool g_notified = false;

void notifyCallback()
{
    std::cout << "Notify callback called!" << std::endl;
    {
        std::lock_guard<std::mutex> lk(g_cv_m);
        g_notified = true;
    }
    g_cv.notify_one();
}

int main(int argc, char* argv[])
{
    HANDLER handler;
    Result result;

    openHandler(handler, result);

    if(!handler || result != Result::SUCCESS)
    {
        std::cerr << "Failed to open handler. " << "Result: " << static_cast<int>(result) << std::endl;
        return -1;
    }

    std::cout << "Handler opened successfully! Handler: " << handler << ". Result: " << static_cast<int>(result) << std::endl;

    setWidgetNotificationCallback(handler, result, notifyCallback);

    startWidget(handler, result);

    {
        std::unique_lock<std::mutex> lk(g_cv_m);

        // Wait max 6 seconds for the callback to be called
        if (!g_cv.wait_for(lk, std::chrono::seconds(6), []{ return g_notified; }))
        {
            std::cerr << "Timeout waiting for callback." << std::endl;
        }
        else
        {
            std::cout << "Callback received." << std::endl;
        }
    }

    closeHandler(handler, result);

    if(handler != nullptr)
    {
        std::cerr << "Failed to close handler. " << handler << "Result: " << static_cast<int>(result) << std::endl;
        return -1;
    }
    
    std::cout << "Handler closed successfully! Handler: " << handler << ". Result: " << static_cast<int>(result) << std::endl;

    return 0;
}