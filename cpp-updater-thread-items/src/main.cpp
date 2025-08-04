#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace cppUpdatarThreadItems
{
    class IUpdater
    {
    public:
        virtual ~IUpdater() = default;
        virtual void startUpdate() = 0;
        virtual void stopUpdate() = 0;

    protected:
        IUpdater() = default;
        IUpdater(const IUpdater&) = default;
        IUpdater& operator=(const IUpdater&) = default;
        IUpdater(IUpdater&&) = default;
        IUpdater& operator=(IUpdater&&) = default;
    };

    class Updater final : public IUpdater
    {
    public:
        Updater() = default;
        ~Updater() override;

        void startUpdate() override;
        void stopUpdate() override;

    private:
        std::thread updateThread_;
        std::mutex mtx_;
        std::condition_variable cv_;
        std::atomic<bool> running_{false};
        std::atomic<bool> stopRequested_{false};

        void runUpdate();
    };

    void Updater::startUpdate()
    {
        if (running_)
        {
            std::cerr << "Updater thread already running." << std::endl;
            return;
        }

        stopRequested_ = false;
        running_ = true;

        updateThread_ = std::thread(&Updater::runUpdate, this);
        std::cout << "I am thead (" << std::this_thread::get_id() << "). Updater thread started." << std::endl;
    }

    void Updater::stopUpdate()
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (!running_) return;
            stopRequested_ = true;
        }
        cv_.notify_one();

        if (updateThread_.joinable()) {
            updateThread_.join();
        }

        running_ = false;
        std::cout << "I am thead (" << std::this_thread::get_id() << "). Updater thread stopped." << std::endl;
    }

    void Updater::runUpdate()
    {
        std::unique_lock<std::mutex> lock(mtx_);

        while (!stopRequested_) 
        {
            // Simulate update work
            std::cout << "I am thead (" << std::this_thread::get_id() << "). Running update..." << std::endl;
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            lock.lock();

            // Wait for stop or timeout (simulate "next work cycle")
            cv_.wait_for(lock, std::chrono::milliseconds(100), [this]() 
            {
                return stopRequested_.load();
            });
        }

        std::cout << "Updater thread exiting gracefully." << std::endl;
    }

    Updater::~Updater()
    {
        stopUpdate();  // Clean shutdown
        std::cout << "Updater destroyed." << std::endl;
    }

    class IDevice
    {
    public:
        virtual ~IDevice() = default;

    protected:
        IDevice() = default;
        IDevice(const IDevice&) = default;
        IDevice& operator=(const IDevice&) = default;
        IDevice(IDevice&&) = default;
        IDevice& operator=(IDevice&&) = default;
    };

    class Device final : public IDevice
    {
    public:
        Device(IUpdater* updater);
        ~Device() override;


    private:
        IUpdater* updater_ = nullptr;
    };

    Device::Device(IUpdater* updater)
    : updater_ (updater)
    {

    }

    Device::~Device()
    {
        if (updater_)
        {
            updater_->stopUpdate();
            delete updater_;
            updater_ = nullptr;
        }
        std::cout << "Device destroyed and updater stopped." << std::endl;
    }

    class IDeviceBuilder
    {
    public:
        virtual ~IDeviceBuilder() = default;
        virtual IDeviceBuilder& setUpdaterThread() = 0;
        virtual IDevice* build() = 0;
    
    protected:
        IDeviceBuilder() = default;
        IDeviceBuilder(const IDeviceBuilder&) = default;
        IDeviceBuilder& operator=(const IDeviceBuilder&) = default;
        IDeviceBuilder(IDeviceBuilder&&) = default;
        IDeviceBuilder& operator=(IDeviceBuilder&&) = default;            
    };

    class DeviceBuilder final : public IDeviceBuilder
    {
    public:
        DeviceBuilder() = default;
        ~DeviceBuilder() override = default;
        IDeviceBuilder& setUpdaterThread() override;
        IDevice* build() override;
    
    private:
        IUpdater* updater_ = nullptr;
    };

    IDevice* DeviceBuilder::build()
    {
        return new Device(updater_);
    }

    IDeviceBuilder& DeviceBuilder::setUpdaterThread()
    {
        if (updater_ == nullptr)
        {
            updater_ = new Updater();
        }

        updater_->startUpdate();

        return *this;
    }

} // nampespace cppUpdatarThreadItems

using namespace cppUpdatarThreadItems;

int main (int argc, char* argv[])
{
    std::vector<std::unique_ptr<IDevice>> devices;
    const int nThreads = 3;

    for (int i = 0; i < nThreads; ++i) 
    {
        auto builder = std::make_unique<DeviceBuilder>();
        devices.emplace_back(builder->setUpdaterThread().build());
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}