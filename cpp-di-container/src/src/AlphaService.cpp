#include <iostream>
#include <memory>
#include <string>
#include "AlphaService.h"

void AlphaService::execute()
{
    // Implementation of the service's functionality

    std::string addressMemory = std::to_string(reinterpret_cast<std::uintptr_t>(this));
    std::string message = addressMemory + " | AlphaService is executing.";

    std::cout << message << std::endl;
}