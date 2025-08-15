#ifndef MAIN_SERVICE_H
#define MAIN_SERVICE_H

#include "IMainService.h"

#include <memory>

class IMultiService;

class MainService final : public IMainService
{
public:

    /**
     * @brief Constructs a MessageService with a specific IMessageSender.
     * DI (Dependency Injection) is used to provide the IMessageSender dependency.
     */
    MainService(std::shared_ptr<IMultiService> multiService);

    ~MainService() = default;

    int computeNumber(int x, int y) override;
    
private:

    std::shared_ptr<IMultiService> multiService_; // Dependency on IMultiService
};

#endif // MAIN_SERVICE_H