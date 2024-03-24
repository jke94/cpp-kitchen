#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include "IPerson.h"

#define PERSON_PPM_MIN  60
#define PERSON_PPM_MAX  180

class Person : public IPerson
{
    private:

        int _milliseconds_notification_time = 1000;
        std::string _name;
        int _ppm;
        std::thread _thread_life;

        void (*_person_ppm_callback)(std::string, int);

        void do_live();

    public:
        Person(std::string name, int milliseconds_notification_time, void (*person_ppm_callback)(std::string, int));

        ~Person();

        std::string get_name() override;
        int get_ppm() override;
};