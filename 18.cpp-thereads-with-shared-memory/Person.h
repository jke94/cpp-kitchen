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
        std::string _name;
        int _ppm;

        std::thread _thread_life;
        void do_live();

    public:
        Person(std::string name);

        ~Person();

        std::string get_name() override;
    
        int get_ppm() override;
        void set_ppm(int ppm) override;
};