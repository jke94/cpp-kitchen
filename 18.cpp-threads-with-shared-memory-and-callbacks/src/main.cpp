#include <thread>
#include <memory>

#include "IMonitorApi.h"

int main()
{
    std::shared_ptr<IMonitor> monitor = create_monitor();

    monitor->add_person("Javi",     1000);
    monitor->add_person("Ale",      250);
    monitor->add_person("Milei",    500);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    monitor->add_person("Lucas",    500);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
}