#include <chrono>
#include <iostream>
#include <random>
#include "Person.h"

std::mutex person_mutex;

void Person::do_live()
{
    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(_milliseconds_notification_time));

        int tmp_ppm = PERSON_PPM_MIN + (std::rand()% (PERSON_PPM_MAX - PERSON_PPM_MIN + 1));
        _ppm = tmp_ppm;
        _person_ppm_callback(_name, tmp_ppm);
    } 
    while (_thread_life.joinable());
}

Person::Person(
    std::string name, 
    int milliseconds_notification_time,
    void (*person_ppm_callback)(std::string, int))
{
    _name = name;
    _milliseconds_notification_time = milliseconds_notification_time;
    _person_ppm_callback = person_ppm_callback;
    _thread_life = std::thread(&Person::do_live, this);

    if(_thread_life.joinable())
    {
        std::cout << "Created thread: "<< _thread_life.get_id() << std::endl;
    } 
};

Person::~Person()
{
    if(_thread_life.joinable())
    {
        std::cout << "Deatached thread: "<< _thread_life.get_id() << std::endl;
    }    
    
    _thread_life.detach();

    if(!_thread_life.joinable())
    {
        std::cout << "Deatached thread successfully." << std::endl;
    }
};

std::string Person::get_name()
{
    std::lock_guard<std::mutex> guard(person_mutex);

    return _name;
}

int Person::get_ppm()
{
    std::lock_guard<std::mutex> guard(person_mutex);

    return _ppm;
}
