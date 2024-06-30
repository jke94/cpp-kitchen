#ifndef HANLDER_V1_H
#define HANLDER_V1_H

#include "IHandler.h"

class HandlerV1 : public IHandler 
{
public:
    void handle(const std::string& xmlData) override;
};

#endif