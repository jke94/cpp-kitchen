#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "IApplication.h"

// Forward declaration
class ILogger;
class IService;

class Application : public IApplication
{
private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IService> service_;

public:
    Application(std::shared_ptr<ILogger> logger, std::shared_ptr<IService> service);
    ~Application() override = default;
    void run() override;
};

#endif // APPLICATION_H