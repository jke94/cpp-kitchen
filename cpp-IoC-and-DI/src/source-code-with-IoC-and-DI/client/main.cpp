#include <iostream>
#include <string>
#include <memory>

#include "MessageService.h"
#include "EmailSender.h"

int main() 
{
    auto emailSender = std::make_unique<EmailSender>();
    
    MessageService service(std::move(emailSender));  // DI + propiedad + RAII
    service.sendMessage("Hola con smart pointer");
}