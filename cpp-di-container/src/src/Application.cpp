#include "Application.h"

#include <cstdint>
#include <memory>
#include <string>

#include "ILogger.h"
#include "IService.h"
#include "AlphaService.h"

Application::Application(
    std::shared_ptr<ILogger> logger,
    std::shared_ptr<IService> service) : 
logger_(std::move(logger)),
service_(std::move(service)) 
{

}

void Application::run() 
{
    std::string addressMemory = std::to_string(reinterpret_cast<std::uintptr_t>(this));
    std::string message = addressMemory + " | Aplicación corriendo desde main";
    
    logger_->log(message);

    service_->execute();
    logger_->log("Servicio ejecutado desde main");
}