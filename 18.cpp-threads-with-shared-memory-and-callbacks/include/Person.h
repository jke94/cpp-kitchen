#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <thread>

#include "MonitorApi.h"
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

        PERSON_MONITOR_CALLBACK _person_ppm_callback = nullptr;

        void do_live();

    public:
        Person(std::string name, int milliseconds_notification_time, void (*person_ppm_callback)(std::string, int));

        ~Person();

        std::string get_name() override;
        int get_ppm() override;
};

#endif