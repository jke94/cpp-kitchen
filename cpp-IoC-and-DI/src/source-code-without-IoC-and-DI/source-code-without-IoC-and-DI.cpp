#include <iostream>
#include <string>

class EmailSender 
{
public:
    void send(std::string message) 
    {
        std::cout << "Enviando email: " << message << std::endl;
    }
};

class MessageService 
{
public:
    void sendMessage(std::string message) 
    {
        EmailSender sender;
        sender.send(message);  // Acoplamiento fuerte
    }
};

int main() 
{
    MessageService service;
    service.sendMessage("Hola mundo");
}
