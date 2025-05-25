#ifndef DEVICE_TRACKER_API_H
#define DEVICE_TRACKER_API_H

#include <string>

namespace device_tracker
{
    enum class DeviceStatusType
    {
        WTF = -1,
        Ready = 0,
        Sleep = 1,
        Error = 2,
    };

    /**
     * @brief Status of device creation.
     */
    enum class DeviceCreationStatus 
    {
        Success,
        Timeout,
        MissingCredentials,
        ConnectionFailed,
        InternalError
    };   

    /**
     * @brief Interface for device information subsystem.
     */
    class IDevideInformation
    {
    public:
        virtual ~IDevideInformation() = default;
        virtual DeviceStatusType getDeviceStatus() const = 0;
        virtual void onDeviceStatusChanged(DeviceStatusType status) = 0;

    protected:
        IDevideInformation() = default;
        IDevideInformation(const IDevideInformation&) = delete;
        IDevideInformation& operator=(const IDevideInformation&) = delete;
        IDevideInformation(IDevideInformation&&) = delete;
        IDevideInformation& operator=(IDevideInformation&&) = delete;
    };

    /**
     * @brief Result of device creation.
     */
    struct DeviceCreationResult 
    {
        DeviceCreationStatus status;
        IDevice* device = nullptr;
    }; 

    struct DeviceConfig 
    {
        std::string hostname;
        std::string username;
        std::string password;
        int timeoutMs;
    };

    class IDevice
    {
    public:
        virtual ~IDevice() = default;
        virtual IDevideInformation* getDeviceInformation() const = 0;
    protected:
        IDevice() = default;
        IDevice(const IDevice&) = delete;
        IDevice& operator=(const IDevice&) = delete;
        IDevice(IDevice&&) = delete;
        IDevice& operator=(IDevice&&) = delete;
    };
    
    /**
     * @brief Attempts to create and initialize a device.
     *
     * @param config Configuration parameters for the device.
     * @return std::future<DeviceCreationResult> Result object indicating success/failure.
     */
    std::future<DeviceCreationResult> createDevice(const DeviceConfig& config);

    /**
     * @brief Destroy a device instance.
     *
     * @param device Pointer to the device instance to be destroyed.
     * @return true if the device was successfully destroyed, false otherwise.
     */
    bool destroyDevice(IDevice* device);

}; // namespace device_tracker

#endif // DEVICE_TRACKER_API_H
