/**
 *  DYNAMIC CASTING MULTITHREADING
 */

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <sstream>
#include <mutex>
#include <random>

// ---------------------------------------------------------------
// --------- API CODE available to be used by the client ---------
// ---------------------------------------------------------------

class IDevice
{
protected:
    virtual ~IDevice() {};
};

void initLogging(void(*logCallback)(const char*));

IDevice* createDevice();
void startDevice(IDevice* device);
void stopDevice(IDevice* device);
void destroyDevice(IDevice* device);

// ---------------------------------------------------------------

// ---------------------------------------------------------------
// ---------------- INTERNAL IMPLEMENTATION ----------------------
// ---------------------------------------------------------------

void(*logCallback_)(const char*) = nullptr;

class IBaseDevice 
{
protected:
    virtual ~IBaseDevice() {};
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

class Device : public IBaseDevice, public IDevice
{
public:
    Device();
    ~Device();

    void start() override;
    void stop() override;

private:

    std::atomic<bool> running;
    std::thread worker;
    int counter_ = 1;
    void run();
    int generateRandomNumberOfSeconds();
};

Device::Device() : 
    running(false),
    counter_(1) 
{ 
}

Device::~Device()
{
    stop();
}

void Device::stop() 
{
    running = false;

    if (worker.joinable()) 
    {
        worker.join();
    }
}

void Device::start() 
{
    running = true;
    worker = std::thread(&Device::run, this);
}

void Device::run()
{
    while (running)
    {
        // Work simulation.
        std::stringstream ss;
        std::ostringstream oss;

        std::thread::id threadId = std::this_thread::get_id();
        oss << std::hex << threadId;

        ss << "I am ";
        ss << threadId << " (0x" << oss.str() << ")";
        ss << " device trabajando. Counter=";
        ss << counter_;

        logCallback_(ss.str().c_str());

        counter_++;
        std::this_thread::sleep_for(std::chrono::milliseconds(generateRandomNumberOfSeconds()));
    }

    std::stringstream ss;
    ss << "Stopped thread: " << std::this_thread::get_id();
    logCallback_(ss.str().c_str());
}

int Device::generateRandomNumberOfSeconds() 
{
    
    std::random_device rd;
    std::mt19937 gen(rd());

    // Range beetween [100, 280]
    std::uniform_int_distribution<> dis(100, 280);

    return dis(gen);
}

void initLogging(void(*logCallback)(const char*))
{
    logCallback_ = logCallback;
}

IDevice* createDevice()
{
    IBaseDevice* device = nullptr;

    device = new Device();
    
    return dynamic_cast<IDevice*>(device);
}

void startDevice(IDevice* device)
{
    dynamic_cast<IBaseDevice*>(device)->start();
}

void stopDevice(IDevice* device)
{
    dynamic_cast<Device*>(device)->stop();
}

void destroyDevice(IDevice* device)
{
    delete dynamic_cast<Device*>(device);
}

std::mutex mutex_;

void OnLogMessage(const char* message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << message << std::endl;
}

int main() 
{
    // A. Init callback to recive messages.
    initLogging(OnLogMessage);
    
    // B. Initialization local values.
    int nDevices = 5;
    std::vector<IDevice*> devices;

    // C. Create devices.
    for (int i = 0; i < nDevices; ++i) 
    {
        IDevice* device = createDevice();
        devices.emplace_back(device);
    }
    
    // D. Start devices.
    for (auto& device : devices) 
    {
        startDevice(device);
    }

    // E. Wait some time after stop threads.
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // F. Stop all devices (thread working on background).
    for (auto& device : devices) 
    {
        stopDevice(device);
    }

    // G . Destroyed devices.
    for(auto& device : devices)
    {
        destroyDevice(device);
    }

    // H. Clear devices vector.
    devices.clear();

    return 0;
}