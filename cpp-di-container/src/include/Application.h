#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "IApplication.h"
#include "ILogger.h"

class Application : public IApplication
{
public:
    Application(std::shared_ptr<ILogger> logger) : logger_(std::move(logger)) {}
    void run() {
        logger_->log("Aplicación corriendo desde main");
    }
private:
    std::shared_ptr<ILogger> logger_;
};

#endif // APPLICATION_H