#include <cstdint>
#include <iostream>

/**
 * @brief API definition for event handling.
 */

namespace api
{
    enum class Result : int
    {
        Success = 0,
        InvalidParameter = 1,
        OperationFailed = 2
    };

    enum class EventType : int
    {
        DeviceConnected = 1,
        DeviceDisconnected = 2,
        ErrorOccurred = 3,

        OnStatusUpdate = 4,
        OnAlertsUpdate = 5
    };

    enum class DeviceStatus : int
    {
        Unknown = -1,
        Ok = 0,
        Warning = 1,
        Error = 2,
        Sleeping = 3
    };

    struct EventData
    {
        int StructSize;
        EventType EventTypeValue;
        const void* Payload;
    };

    struct OnStatusUpdate
    {
        int StructSize;
        DeviceStatus status;
    };

    struct OnAlertsUpdate
    {
        int StructSize;
        int alertsCount;
    };

    struct DeviceConnectedInfo
    {
        int StructSize;
        const char* IpAddress;
        const char* Model;
    };

    struct DeviceDisconnectedInfo
    {
        int StructSize;
        const char* IpAddress;
    };

    struct ErrorInfo
    {
        int StructSize;
        int ErrorCode;
        const char* Message;
    };


    using EventCallback = void (*)(const EventData& event);

    extern "C" void dispatchEvent(const EventData& event, EventCallback callback, Result& result);

} // namespace api

std::ostream& operator<<(std::ostream& os, const api::Result& result);
std::ostream& operator<<(std::ostream& os, const api::DeviceStatus& status);

/**
 * @brief Callback function from client side to manage event from own bussiness logic (on this example is just print to console).
 */
void OnEventCallback(const api::EventData& event)
{
    switch (event.EventTypeValue)
    {
        case api::EventType::DeviceConnected:
        {
            const auto* payload = static_cast<const api::DeviceConnectedInfo*>(event.Payload);
            std::cout << "[Client] Device connected: " << payload->IpAddress
                      << " / " << payload->Model << std::endl;
            break;
        }
        case api::EventType::DeviceDisconnected:
        {
            const auto* payload = static_cast<const api::DeviceDisconnectedInfo*>(event.Payload);
            std::cout << "[Client] Device disconnected: " << payload->IpAddress << std::endl;
            break;
        }
        case api::EventType::ErrorOccurred:
        {
            const auto* payload = static_cast<const api::ErrorInfo*>(event.Payload);
            std::cout << "[Client] Error " << payload->ErrorCode << ": " << payload->Message << std::endl;
            break;
        }
        case api::EventType::OnStatusUpdate:
        {
            const auto* payload = static_cast<const api::OnStatusUpdate*>(event.Payload);
            std::cout << "[Client] Status update: " << payload->status << std::endl;
            break;
        }
        case api::EventType::OnAlertsUpdate:
        {
            const auto* payload = static_cast<const api::OnAlertsUpdate*>(event.Payload);
            std::cout << "[Client] Alerts update: " << payload->alertsCount << std::endl;
            break;
        }
        default:
            std::cout << "[Client] Unhandled event type\n";
            break;
    }
}

/**
 * @brief Main function to simulate event dispatching.
 */
int main(int argc, char* argv[])
{
    // Simulate dispatching a DeviceConnected event
    api::DeviceConnectedInfo connectedInfo{};
    connectedInfo.StructSize = sizeof(api::DeviceConnectedInfo);
    connectedInfo.IpAddress = "192.168.1.10";
    connectedInfo.Model = "Device-01";

    api::EventData connectedEvent{};
    connectedEvent.StructSize = sizeof(api::EventData);
    connectedEvent.EventTypeValue = api::EventType::DeviceConnected;
    connectedEvent.Payload = &connectedInfo;

    api::Result connectedResult;
    api::dispatchEvent(connectedEvent, OnEventCallback, connectedResult);

    // Simulate dispatching a DeviceDisconnected event

    api::DeviceDisconnectedInfo disconnectedInfo{};
    disconnectedInfo.StructSize = sizeof(api::DeviceDisconnectedInfo);
    disconnectedInfo.IpAddress = "192.168.1.10";

    api::EventData disconnectedEvent{};
    disconnectedEvent.StructSize = sizeof(api::EventData);
    disconnectedEvent.EventTypeValue = api::EventType::DeviceDisconnected;
    disconnectedEvent.Payload = &disconnectedInfo;

    api::Result disconnectedResult;
    api::dispatchEvent(disconnectedEvent, OnEventCallback, disconnectedResult);

    // Simulate dispatching an ErrorOccurred event

    api::ErrorInfo errorInfo{};
    errorInfo.StructSize = sizeof(api::ErrorInfo);
    errorInfo.ErrorCode = 500;
    errorInfo.Message = "Connection timeout";

    api::EventData errorEvent{};
    errorEvent.StructSize = sizeof(api::EventData);
    errorEvent.EventTypeValue = api::EventType::ErrorOccurred;
    errorEvent.Payload = &errorInfo;

    api::Result errorResult;
    api::dispatchEvent(errorEvent, OnEventCallback, errorResult);

    // Simulate dispatching an OnStatusUpdate event

    api::OnStatusUpdate statusUpdateInfo{};
    statusUpdateInfo.StructSize = sizeof(api::OnStatusUpdate);
    statusUpdateInfo.status = api::DeviceStatus::Ok;

    api::EventData statusUpdateEvent{};
    statusUpdateEvent.StructSize = sizeof(api::EventData);
    statusUpdateEvent.EventTypeValue = api::EventType::OnStatusUpdate;
    statusUpdateEvent.Payload = &statusUpdateInfo;

    api::Result statusUpdateResult;
    api::dispatchEvent(statusUpdateEvent, OnEventCallback, statusUpdateResult);

    // Simulate dispatching an OnAlertsUpdate event

    api::OnAlertsUpdate alertsUpdateInfo{};
    alertsUpdateInfo.StructSize = sizeof(api::OnAlertsUpdate);
    alertsUpdateInfo.alertsCount = 5;

    api::EventData alertsUpdateEvent{};
    alertsUpdateEvent.StructSize = sizeof(api::EventData);
    alertsUpdateEvent.EventTypeValue = api::EventType::OnAlertsUpdate;
    alertsUpdateEvent.Payload = &alertsUpdateInfo;

    api::Result alertsUpdateResult;
    api::dispatchEvent(alertsUpdateEvent, OnEventCallback, alertsUpdateResult);

    return 0;
}

std::ostream& operator<<(std::ostream& os, const api::Result& result)
{
    switch (result)
    {
        case api::Result::Success:
            os << "Success";
            break;
        case api::Result::InvalidParameter:
            os << "InvalidParameter";
            break;
        case api::Result::OperationFailed:
            os << "OperationFailed";
            break;
        default:
            os << "Unknown Result";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const api::DeviceStatus& status)
{
    switch (status)
    {
        case api::DeviceStatus::Ok:
            os << "Ok";
            break;
        case api::DeviceStatus::Error:
            os << "Error";
            break;
        case api::DeviceStatus::Sleeping:
            os << "Sleeping";
            break;
        case api::DeviceStatus::Unknown:
            os << "Unknown";
            break;
        default:
            os << "Unknown Status";
            break;
    }
    return os;
}

/**
 * @brief API implementation for event handling.
 */
namespace api
{
    void dispatchEvent(const api::EventData& event, EventCallback callback, Result& result)
    {
        if (callback != nullptr)
        {
            callback(event);
            result = Result::Success;
        }
        else
        {
            result = Result::InvalidParameter;
        }
    }
} // namespace api