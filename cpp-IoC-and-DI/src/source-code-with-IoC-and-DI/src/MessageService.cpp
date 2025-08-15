#include "MessageService.h"

#include <memory>
#include <string>

#include "IMessageSender.h"

MessageService::MessageService(std::unique_ptr<IMessageSender> s) : 
    sender_(std::move(s)) 
{
    
}

void MessageService::sendMessage(const std::string& message) 
{
    if (sender_)
    {
        sender_->send(message);
    }
}