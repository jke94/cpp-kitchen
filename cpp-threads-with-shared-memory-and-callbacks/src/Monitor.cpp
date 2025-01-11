#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> 

#include "Monitor.h"
#include "Person.h"

std::mutex monitor_mutex;

std::string get_date_and_time()
{
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
    const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::stringstream nowSs{};
      
#if defined(_WIN32) || defined(__CYGWIN__)

    time_t rawtime = std::time(nullptr);
    struct tm timeinfo;
    auto lc_time =  localtime_s(&timeinfo, &rawtime);

    nowSs << std::put_time(&timeinfo, "%Y-%m-%d %X")
    << '.' << std::setfill('0') << std::setw(3) << nowMs.count();

#elif defined(__linux__)

    nowSs << std::put_time(std::localtime(&nowAsTimeT), "%Y-%m-%d %X")
        << '.' << std::setfill('0') << std::setw(3) << nowMs.count();
#else
    nowSs << "[ERROR_GET_TIME]";
#endif

    return nowSs.str();
}

void show_ppm_person(std::string name, int ppm)
{
    std::lock_guard<std::mutex> guard(monitor_mutex);

    std::cout << "[" << get_date_and_time() <<"] Name: " << name << ", PPM: " << ppm << std::endl;
}

Monitor::Monitor()
{
    _monitor = std::vector<std::shared_ptr<IPerson>>();
    _person_ppm_callback = &show_ppm_person;
}

Monitor::~Monitor()
{

}

void Monitor::add_person(std::string name, int time_to_update)
{
    std::lock_guard<std::mutex> guard(monitor_mutex);
    _monitor.push_back(std::make_shared<Person>(name, time_to_update, _person_ppm_callback));
}

void Monitor::remove_person(std::string name)
{
    std::lock_guard<std::mutex> guard(monitor_mutex);

    auto item = find_if(_monitor.begin(), _monitor.end(), [&name](const std::shared_ptr<IPerson> person) 
        {
            return person->get_name() == name;
        });

    if(item->get())
    {
        _monitor.erase(item);
    }
}
