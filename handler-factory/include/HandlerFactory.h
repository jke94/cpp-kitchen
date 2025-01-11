#ifndef HANDLER_FACTORY_H
#define HANDLER_FACTORY_H

#include "IHandler.h"
#include "HandlerV1.h"
#include "HandlerV2.h"
#include <string>
#include <stdexcept>
#include <memory>

class HandlerFactory 
{
public:
    static std::shared_ptr<IHandler> getHandler(const std::string& version);
};

#endif