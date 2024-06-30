#ifndef HANLDER_V2_H
#define HANLDER_V2_H

#include "IHandler.h"

class HandlerV2 : public IHandler 
{
public:
    void handle(const std::string& xmlData) override;
};

#endif