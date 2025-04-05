#include <memory>
#include <string>

#include "Application.h"
#include "ILogger.h"

Application::Application(
    std::shared_ptr<ILogger> logger
) : 
logger_(std::move(logger)) 
{

}

void Application::run() 
{
    std::string addressMemory = std::to_string(reinterpret_cast<std::uintptr_t>(this));
    std::string message = addressMemory + " | Aplicación corriendo desde main";
    
    logger_->log(message);
}