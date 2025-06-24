#include "SMSSender.h"

#include <iostream>

void SMSSender::send(std::string message) 
{
    std::cout << "Enviando SMS: " << message << std::endl;
}