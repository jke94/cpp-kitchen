#include "IMonitorApi.h"
#include "Monitor.h"

std::shared_ptr<IMonitor> create_monitor()
{
    return std::make_shared<Monitor>();
}