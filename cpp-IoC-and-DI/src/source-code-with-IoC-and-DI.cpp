#include <iostream>
#include <string>
#include <memory>

class IMessageSender
{
public:
    virtual void send(std::string message) = 0;
    virtual ~IMessageSender() = default;
};

class EmailSender final : public IMessageSender 
{
public:
    void send(std::string message) override 
    {
        std::cout << "Enviando email: " << message << std::endl;
    }
};

class SMSSender final : public IMessageSender 
{
public:
    void send(std::string message) override 
    {
        std::cout << "Enviando SMS: " << message << std::endl;
    }
};

class MessageService 
{
public:
    // DI por constructor con propiedad
    MessageService(std::unique_ptr<IMessageSender> s)
        : sender(std::move(s)) {}

    void sendMessage(const std::string& message) 
    {
        if (sender)
        {
            sender->send(message);
        }
    }
private:
    std::unique_ptr<IMessageSender> sender;    
};

int main() 
{
    auto emailSender = std::make_unique<EmailSender>();
    
    MessageService service(std::move(emailSender));  // DI + propiedad + RAII
    service.sendMessage("Hola con smart pointer");
}