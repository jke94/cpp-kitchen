#ifndef IMONITOR_API_H
#define IMONITOR_API_H

#include <memory>
#include <string>
#include <vector>

class IMonitor
{
    public:
        virtual ~IMonitor() {};
        virtual void add_person(std::string name, int time_to_update) = 0;
        virtual void remove_person(std::string name) = 0;
};

typedef void (*PERSON_MONITOR_CALLBACK)(std::string, int);

std::shared_ptr<IMonitor> create_monitor();

#endif