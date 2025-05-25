#include <future>
#include <iostream>
#include <string>


#include "DeviceTrackerApi.h"

using namespace device_tracker;

int main(int argc, char* argv[]) 
{
    DeviceConfig config;

    config.hostname = "192.168.1.50";
    config.username = "admin";
    config.password = "1234";
    config.timeoutMs = 4000;

    std::future<DeviceCreationResult> futureDevice = createDevice(config);
    DeviceCreationResult result = futureDevice.get();

    IDevice* device = nullptr;

    switch (result.status)
    {
    case DeviceCreationStatus::Success:
        device = result.device;
        std::cout << "Dispositivo inicializado con éxito.\n";
        
        // TODO: Play with the device.
        
        destroyDevice(device);        
        
        break;

    case DeviceCreationStatus::MissingCredentials:
        std::cerr << "Faltan credenciales para este dispositivo.\n";
        break;
    case DeviceCreationStatus::Timeout:
        std::cerr << "Inicialización del dispositivo excedió el tiempo de espera.\n";
        break;
    default:
        std::cerr << "Error al inicializar el dispositivo.\n";
        break;
    }

    return 0;
}