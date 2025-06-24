#ifndef SMS_SENDER_H
#define SMS_SENDER_H

#include "IMessageSender.h"

class SMSSender final : public IMessageSender 
{
public:
    void send(std::string message) override;
};

#endif // SMS_SENDER_H