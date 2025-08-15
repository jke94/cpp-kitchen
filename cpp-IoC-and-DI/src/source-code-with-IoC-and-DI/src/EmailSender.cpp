#include "EmailSender.h"

#include <iostream>

void EmailSender::send(std::string message) 
{
    std::cout << "Enviando email: " << message << std::endl;
}