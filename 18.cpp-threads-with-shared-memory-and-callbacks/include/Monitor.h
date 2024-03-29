#ifndef MONITOR_H
#define MONITOR_H

#include <vector>
#include <memory>

#include "MonitorApi.h"
#include "IPerson.h"
#include "IMonitor.h"

class Monitor : public IMonitor
{
    private:
        
        std::vector<std::shared_ptr<IPerson>> _monitor;

        // std::string get_date_and_time();
        // void show_ppm_person(std::string name, int ppm);
        PERSON_MONITOR_CALLBACK _person_ppm_callback = nullptr;

    public:
        Monitor();
        ~Monitor();
        void add_person(std::string name, int time_to_update) override;
};

#endif