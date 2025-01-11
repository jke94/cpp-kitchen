#include <memory>
#include <iostream>
#include <future>
#include <chrono>

using namespace std::chrono_literals;

class IAutomobile
{
public:
    virtual std::string turn_on_engine() = 0;
};

class Automobile : public IAutomobile
{
public:
    std::string turn_on_engine() override
    {
        return "Engine turned on.";
    };
};

std::shared_ptr<IAutomobile> get_automobile()
{
    return std::make_shared<Automobile>();
}

int main()
{
    std::cout << "Waiting until the engine starts" << std::flush;

    // Async 
    std::future<std::shared_ptr<IAutomobile>> result = std::async([]() 
    {
        std::this_thread::sleep_for(3.5s);

        return get_automobile();
    });

    // Wait to future action finish
    do
    {
        std::cout << "." << std::flush;
    } 
    while(result.wait_for(250ms) != std::future_status::ready);

    std::cout << std::endl;
    
    // Result processing.
    std::shared_ptr<IAutomobile> automobile = result.get();
    std::cout << automobile.get()->turn_on_engine() << std::endl;

    return 0;
}