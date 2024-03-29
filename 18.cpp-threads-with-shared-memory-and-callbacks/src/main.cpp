#include <thread>
#include <memory>

#include "Monitor.h"
#include "IMonitor.h"

int main()
{
    std::shared_ptr<IMonitor> monitor = std::make_shared<Monitor>();

    monitor->add_person("Javi",     1000);
    monitor->add_person("Ale",      250);
    monitor->add_person("Milei",    500);

    std::this_thread::sleep_for(std::chrono::seconds(8));   

    return 0;
}