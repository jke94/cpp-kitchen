#include "DeviceTrackerApi.h"

#include <future>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "Device.h"

namespace device_tracker
{
    // Contenedor global para dispositivos activos
    static std::unordered_map<std::string, IDevice*> g_devices;
    static std::mutex g_devicesMutex;

    std::future<DeviceCreationResult> createDevice(const DeviceConfig& config) 
    {
        std::promise<DeviceCreationResult> promise;
        auto future = promise.get_future();

        {
            std::lock_guard<std::mutex> lock(g_devicesMutex);
            auto it = g_devices.find(config.hostname);
            if (it != g_devices.end()) {
                // Ya existe dispositivo para ese hostname
                DeviceCreationResult result;
                result.status = DeviceCreationStatus::Success;
                result.device = it->second;
                promise.set_value(result);
                return future;
            }
        }

        // No existe aún, creamos nuevo dispositivo en hilo separado
        std::thread([config, p = std::move(promise)]() mutable {
            DeviceCreationResult result;
            
            std::unique_ptr<Device> device(new Device());

            bool initialized = device->initialize(config);

            if (!initialized) 
            {
                // Según último estado guardado en device, devolvemos error apropiado
                result.status = device->getLastInitStatus(); // método que debe devolver DeviceCreationStatus
                result.device = nullptr;
                p.set_value(result);
                return;
            }

            result.status = DeviceCreationStatus::Success;
            result.device = device.get();

            {
                std::lock_guard<std::mutex> lock(g_devicesMutex);
                // Guardamos dispositivo para evitar duplicados
                g_devices[config.hostname] = device.release();
            }

            p.set_value(result);

        }).detach();

        return future;
    }

    bool destroyDevice(IDevice* device) 
    {
        if (!device) return false;

        std::string hostname;

        {
            std::lock_guard<std::mutex> lock(g_devicesMutex);

            // Buscar en el mapa para conocer el hostname asociado al device
            auto it = std::find_if(g_devices.begin(), g_devices.end(),
                [device](const auto& pair) {
                    return pair.second == device;
                });

            if (it == g_devices.end()) 
            {
                // No está en el contenedor => no es un device válido o ya destruido
                return false;
            }

            hostname = it->first;
            g_devices.erase(it);
        }

        // Destruir el dispositivo
        delete device;

        return true;
    }    
};