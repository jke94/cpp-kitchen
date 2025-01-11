#ifndef MONITOR_H
#define MONITOR_H

#include <vector>
#include <memory>

#include "IMonitorApi.h"
#include "IPerson.h"

class Monitor : public IMonitor
{
    private:
        
        std::vector<std::shared_ptr<IPerson>> _monitor;

        PERSON_MONITOR_CALLBACK _person_ppm_callback = nullptr;

    public:
        Monitor();
        ~Monitor();
        void add_person(std::string name, int time_to_update) override;
        void remove_person(std::string name) override;
};

#endif