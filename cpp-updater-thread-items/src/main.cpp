#include <atomic>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

namespace cppUpdatarThreadItemsLogger
{
    enum class LOG_LEVEL
    {
        INFO,
        WARNING,
        ERROR,
        CRITICAL,
        VERBOSE
    };

    template <typename... Args>
    std::string build_message(const Args&... args) 
    {
        std::ostringstream stream;
        int dummy[] = {0, (stream << args, 0)...};
        return stream.str();
    }

    std::ostream &operator<<(std::ostream &os, LOG_LEVEL level);
    std::string currentDateTime();
    void log(const char *file, const char *function, const int line, const std::string &message, LOG_LEVEL logLevel);
    void logInfo(const char* file, const char* function, const int line, const std::string& message);

    #define LOG_CONSOLE_INFO(...) logInfo(__FILE__, __FUNCTION__, __LINE__, build_message(__VA_ARGS__))

}; // namespace cppUpdatarThreadItemsLogger


namespace cppUpdatarThreadItems
{
    class IUpdatable 
    {
    public:
        virtual ~IUpdatable() = default;
        virtual void onUpdate() = 0;
    };

    class Updatable : public IUpdatable
    {
    public:
        void onUpdate() override;
    };

    class IUpdater
    {
    public:
        virtual ~IUpdater() = default;
        virtual void startUpdate() = 0;
        virtual void setUpdatable(IUpdatable* updatable) = 0;
        virtual void stopUpdate() = 0;

    protected:
        IUpdater() = default;
        IUpdater(const IUpdater&) = default;
        IUpdater& operator=(const IUpdater&) = default;
        IUpdater(IUpdater&&) = default;
        IUpdater& operator=(IUpdater&&) = default;
    };

    class Updater final : public IUpdater, public IUpdatable
    {
    public:

        /**
         * @brief Constructor for Updater.
         * @param pollingTime Time in milliseconds to wait before next update cycle.
         * @note Default polling time is 3000 milliseconds.
         */
        Updater(int pollingTime);
        ~Updater() override;

        void startUpdate() override;
        void setUpdatable(IUpdatable* updatable) override;
        void stopUpdate() override;
        
        
        void onUpdate() override;

    private:
        std::thread updateThread_;
        std::mutex mtx_;
        std::condition_variable cv_;
        std::atomic<bool> running_{false};
        std::atomic<bool> stopRequested_{false};

        IUpdatable* updatable_ = nullptr;

        /**
         * @brief Time in milliseconds to wait before next update cycle in milliseconds.
         */
        int pollingTime_ = 3000; 

        /**
         * @brief Function that runs in a separate thread to perform updates.
         */
        void runUpdate();
    };

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
        Device(int pollingTime, IUpdater* updater);
        ~Device() override;

    private:
        IUpdater* updater_ = nullptr;
        IUpdatable* updatable_ = nullptr;
        int pollingTime_;
    };

    class IDeviceBuilder
    {
    public:
        virtual ~IDeviceBuilder() = default;
        virtual IDeviceBuilder& setPollingTimeInMs(int pollingTime) = 0;
        virtual IDeviceBuilder& setUpdatable(IUpdatable* updatable) = 0;
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
        IDeviceBuilder& setPollingTimeInMs(int pollingTime) override;
        IDeviceBuilder& setUpdatable(IUpdatable* updatable) override;
        IDevice* build() override;
    
    private:
        IUpdater* updater_ = nullptr;
        IUpdatable* updatable_ = nullptr;
        int pollingTime_;
    };

} // nampespace cppUpdatarThreadItems

using namespace cppUpdatarThreadItems;

int main (int argc, char* argv[])
{
    std::vector<std::unique_ptr<IDevice>> devices;
    const int nThreads = 3;

    for (int i = 0; i < nThreads; ++i) 
    {
        auto deviceBuilder = std::make_unique<DeviceBuilder>();

        IDevice* device = deviceBuilder->setPollingTimeInMs(3000)
                            .setUpdatable(new Updatable)
                            .build();

        devices.emplace_back(device);
    }

    std::cout << "Created " << devices.size() << " devices." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(7));

    return 0;
}

namespace cppUpdatarThreadItemsLogger
{
    std::mutex mutex_;

    void log(const char *file, const char *function, const int line, const std::string &message, LOG_LEVEL logLevel)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::stringstream ss;

        ss << "[" << currentDateTime() << "]";
        ss << "[" << file << "|" << function << "|" << line << "]";
        ss << " " << message;

        std::cout << "[" << logLevel << "]"<< ss.str() << std::endl;
    }

    void logInfo(const char* file, const char* function, const int line, const std::string& message)
    {
        log(file, function, line, message, LOG_LEVEL::INFO);
    }

    std::ostream &operator<<(std::ostream &os, LOG_LEVEL level)
    {
        switch (level)
        {
            case LOG_LEVEL::INFO: return os << "INFO";
            case LOG_LEVEL::WARNING: return os << "WARNING";
            case LOG_LEVEL::ERROR: return os << "ERROR";
            case LOG_LEVEL::CRITICAL: return os << "CRITICAL";
            case LOG_LEVEL::VERBOSE: return os << "VERBOSE";
            default: return os << "UNKNOWN";
        }
    }

    std::string currentDateTime()
    {
        using namespace std::chrono;
    
        // Obtener el tiempo actual del sistema
        auto now = system_clock::now();
    
        // Obtener la parte de tiempo como time_t para convertir a std::tm
        std::time_t now_time_t = system_clock::to_time_t(now);
    
        // Obtener los milisegundos
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    
        // Convertir time_t a std::tm de forma segura (portátil)
        std::tm tm_snapshot;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm_snapshot, &now_time_t);  // Windows
    #else
        localtime_r(&now_time_t, &tm_snapshot);  // POSIX
    #endif
    
        // Construir string con formato deseado
        std::ostringstream oss;
        oss << std::put_time(&tm_snapshot, "%Y-%m-%d_%H:%M:%S");
        oss << '.' << std::setw(3) << std::setfill('0') << ms.count();
    
        return oss.str();
    }     
} // namespace cppUpdatarThreadItemsLogger

namespace cppUpdatarThreadItems
{
    using namespace cppUpdatarThreadItemsLogger;

    void Updatable::onUpdate()
    {
        // Implement the update logic here.
        LOG_CONSOLE_INFO("I am thead (", std::this_thread::get_id(), "). Performing update.");
    }

    Updater::Updater(int pollingTime)
    : pollingTime_(pollingTime)
    {
    }

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
        
        LOG_CONSOLE_INFO("I am thead (", std::this_thread::get_id(), "). Updater thread started.");
    }

    void Updater::setUpdatable(IUpdatable* updatable)
    {
        updatable_ = updatable;
    }

    void Updater::stopUpdate()
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            if (!running_) return;
            stopRequested_ = true;
        }
        cv_.notify_one();

        if (updateThread_.joinable()) 
        {
            updateThread_.join();
        }

        running_ = false;

        LOG_CONSOLE_INFO("I am thead (", std::this_thread::get_id(), "). Updater thread stopped.");
    }

    void Updater::onUpdate()
    {
        LOG_CONSOLE_INFO("I am thead (", std::this_thread::get_id(), "). Performing update.");
    }

    void Updater::runUpdate()
    {
        std::unique_lock<std::mutex> lock(mtx_);

        while (!stopRequested_)
        {
            // Simulate update work
            LOG_CONSOLE_INFO("I am thead (", std::this_thread::get_id(), "). Running update with polling time: ", pollingTime_, " ms");
            lock.unlock();

            if (updatable_)
            {
                updatable_->onUpdate();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(pollingTime_));
            lock.lock();

            // Wait for stop or timeout (simulate "next work cycle")
            cv_.wait_for(lock, std::chrono::milliseconds(100), [this]() 
            {
                return stopRequested_.load();
            });
        }

        LOG_CONSOLE_INFO("I am thead (", std::this_thread::get_id(), "). Updater thread exiting gracefully.");
    }

    Updater::~Updater()
    {
        stopUpdate();  // Clean shutdown
        LOG_CONSOLE_INFO("Updater destroyed.");

        if (updatable_)
        {
            delete updatable_;
            updatable_ = nullptr;
        }
    }

    Device::Device(int pollingTime, IUpdater* updater) : 
        pollingTime_(pollingTime), 
        updater_ (updater)
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

        LOG_CONSOLE_INFO("Device destroyed and updater stopped.");
    }

    IDevice* DeviceBuilder::build()
    {
        if (pollingTime_ <= 0)
        {
            LOG_CONSOLE_INFO("Cannot build device with non-positive polling time.");
            return nullptr; // Cannot build device with non-positive polling time.
        }

        if(updatable_ == nullptr)
        {
            LOG_CONSOLE_INFO("Cannot build device without an updatable.");
            return nullptr; // Cannot build device without an updater.
        }

        updater_ = new Updater(pollingTime_);
        updater_->startUpdate();
        updater_->setUpdatable(updatable_);

        return new Device(pollingTime_, updater_);
    }

    IDeviceBuilder& DeviceBuilder::setPollingTimeInMs(int pollingTime)
    {
        pollingTime_ = pollingTime;

        return *this;
    }

    IDeviceBuilder& DeviceBuilder::setUpdatable(IUpdatable* updatable)
    {
        updatable_ = updatable;

        return *this;
    }

} // namespace cppUpdatarThreadItems
