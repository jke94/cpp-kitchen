#include "HandlerFactory.h"

std::shared_ptr<IHandler> HandlerFactory::getHandler(const std::string& version) 
{
    if (version == "1.0")
    {
        return std::make_shared<HandlerV1>();
    } 
    else if (version == "2.0") 
    {
        return std::make_shared<HandlerV2>();
    } 
    else 
    {
        throw std::invalid_argument("Unsupported version");
    }
}