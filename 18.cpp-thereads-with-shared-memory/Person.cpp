#include <random>
#include "Person.h"

std::mutex person_mutex;

void Person::do_live()
{
    int count = 0;
    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        int tmp_ppm = PERSON_PPM_MIN + (std::rand()% (PERSON_PPM_MAX - PERSON_PPM_MIN + 1));
        std::cout << "\tActual PPM (" << count << "): " << tmp_ppm << std::endl;

        this->set_ppm(tmp_ppm);
        count++;
    } 
    while (_thread_life.joinable());
}

Person::Person(std::string name)
{
    _name = name;
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
    return _name;
}

int Person::get_ppm()
{
    std::lock_guard<std::mutex> guard(person_mutex);

    return _ppm;
}

void Person::set_ppm(int ppm)
{
    std::lock_guard<std::mutex> guard(person_mutex);

    _ppm = ppm;
}
