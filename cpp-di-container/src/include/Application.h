#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "IApplication.h"
#include "ILogger.h"

class Application : public IApplication
{
public:
    Application(std::shared_ptr<ILogger> logger) : logger_(std::move(logger)) {}
    void run() 
    {
        std::string addressMemory = std::to_string(reinterpret_cast<std::uintptr_t>(this));
        std::string message = addressMemory + " | Aplicación corriendo desde main";
        
        logger_->log(message);
    }
private:
    std::shared_ptr<ILogger> logger_;
};

#endif // APPLICATION_H