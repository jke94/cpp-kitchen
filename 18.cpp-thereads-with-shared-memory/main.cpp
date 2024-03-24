#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "Person.h"

void show_ppm_person(int ppm) 
{
    std::cout << "SHOW PPM: " << ppm << std::endl;
}

int main()
{
    std::shared_ptr<IPerson> person_a = std::make_shared<Person>("Javi");
    
    
    void (*person_ppm_callback)(int);

    int seconds_counter = 0;
    int duration_time = 7;

    do
    {
        seconds_counter++;

        std::cout << seconds_counter << " SHOW PPM: " << person_a->get_ppm() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    } 
    while (seconds_counter < duration_time);
    

    return 0;
}