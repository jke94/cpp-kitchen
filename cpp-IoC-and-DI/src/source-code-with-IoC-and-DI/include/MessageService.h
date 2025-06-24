#ifndef MESSAGE_SERVICE_H
#define MESSAGE_SERVICE_H

#include <memory>

#include "IMessageService.h"

class IMessageSender;

class MessageService final : public IMessageService
{
public:

    /**
     * @brief Constructs a MessageService with a specific IMessageSender.
     * DI (Dependency Injection) is used to provide the IMessageSender dependency.
     */
    MessageService(std::unique_ptr<IMessageSender> s);

    void sendMessage(const std::string& message) override;
    
private:
    std::unique_ptr<IMessageSender> sender_;    
};

#endif // MESSAGE_SERVICE_H