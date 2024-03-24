#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Person.h"

std::mutex main_mutex;

std::string get_date_and_time();
void show_ppm_person(std::string name, int ppm); 

int main()
{
    void (*person_ppm_callback)(std::string, int);
    person_ppm_callback = &show_ppm_person;

    std::vector<std::shared_ptr<IPerson>> persons = {
        std::make_shared<Person>("Javi",    1000,   person_ppm_callback),
        std::make_shared<Person>("Ale",     250,    person_ppm_callback),
        std::make_shared<Person>("Milei",   500,    person_ppm_callback),
    };

    std::this_thread::sleep_for(std::chrono::seconds(8));    

    return 0;
}

void show_ppm_person(std::string name, int ppm) 
{
    std::lock_guard<std::mutex> guard(main_mutex);

    std::cout << "[" << get_date_and_time() <<"] Name: " << name << ", PPM: " << ppm << std::endl;
}

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