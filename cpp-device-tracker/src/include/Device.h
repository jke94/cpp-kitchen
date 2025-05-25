#ifndef DEVICE_TRACKER_H
#define DEVICE_TRACKER_H

#include "DeviceTrackerApi.h"

namespace device_tracker
{
    class Device final: public IDevice
    {
    public:
        Device() = default;
        bool initialize(const DeviceConfig& config);
        IDevideInformation* getDeviceInformation() const override;
    };
};

#endif // DEVICE_TRACKER_H