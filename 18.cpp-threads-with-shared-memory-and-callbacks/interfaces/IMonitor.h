#ifndef IMONITOR_H
#define IMONITOR_H

#include <memory>
#include <string>
#include <vector>

class IMonitor
{
    public:
        virtual ~IMonitor() {};
        virtual void add_person(std::string name, int time_to_update) = 0;
};

#endif