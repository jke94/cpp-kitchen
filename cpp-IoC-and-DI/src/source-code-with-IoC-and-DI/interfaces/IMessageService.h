#ifndef I_MESSAGE_SERVICE_H
#define I_MESSAGE_SERVICE_H

#include <string>

class IMessageService
{
public:
    virtual void sendMessage(const std::string& message) = 0;
    virtual ~IMessageService() = default;

protected:
    /**
     * @brief Default constructor for IMessageService.
     * This constructor is protected to prevent direct instantiation of the interface.
     */
    IMessageService() = default;

    /**
     * @brief Default destructor for IMessageService.
     * This destructor is virtual to ensure proper cleanup of derived classes.
     */
    IMessageService(const IMessageService&) = default;

    /**
     * @brief Default move constructor for IMessageService.
     * This allows derived classes to be moved efficiently.
     */
    IMessageService& operator=(const IMessageService&) = default;

    /**
     * @brief Default move assignment operator for IMessageService.
     * This allows derived classes to be moved efficiently.
     */
    IMessageService(IMessageService&&) = default;
};

#endif // I_MESSAGE_SERVICE_H