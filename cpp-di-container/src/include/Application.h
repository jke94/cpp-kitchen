#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include "IApplication.h"

// Forward declaration
class ILogger;

class Application : public IApplication
{
public:
    Application(std::shared_ptr<ILogger> logger);
    ~Application() override = default;
    void run() override;
private:
    std::shared_ptr<ILogger> logger_;
};

#endif // APPLICATION_H