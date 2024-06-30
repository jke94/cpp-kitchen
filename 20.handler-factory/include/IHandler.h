#ifndef I_HANDLER_H
#define I_HANDLER_H

#include <string>

class IHandler 
{
public:
    virtual void handle(const std::string& xmlData) = 0;
    virtual ~IHandler() = default;
};

#endif