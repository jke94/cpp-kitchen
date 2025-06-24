#ifndef I_MESSAGE_SENDER_H
#define I_MESSAGE_SENDER_H

#include <string>

class IMessageSender
{
public:
    virtual void send(std::string message) = 0;
    virtual ~IMessageSender() = default;

protected:

    /**
     * @brief Default constructor for IMessageSender.
     * This constructor is protected to prevent direct instantiation of the interface.
     */
    IMessageSender() = default;

    /**
     * @brief Default destructor for IMessageSender.
     * This destructor is virtual to ensure proper cleanup of derived classes.
     */
    IMessageSender(const IMessageSender&) = default;

    /**
     * @brief Default move constructor for IMessageSender.
     * This allows derived classes to be moved efficiently.
     */
    IMessageSender& operator=(const IMessageSender&) = default;

    /**
     * @brief Default move assignment operator for IMessageSender.
     * This allows derived classes to be moved efficiently.
     */
    IMessageSender(IMessageSender&&) = default;
};

#endif // I_MESSAGE_SENDER_H