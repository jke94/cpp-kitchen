#ifndef I_EMAIL_SENDER_H
#define I_EMAIL_SENDER_H

#include <string>

#include "IMessageSender.h"

class EmailSender final : public IMessageSender 
{
public:
    EmailSender() = default;
    void send(std::string message) override;
};

#endif // I_EMAIL_SENDER_H