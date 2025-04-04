#ifndef OTHER_MODULE_H
#define OTHER_MODULE_H

#include "DIContext.h"
#include "ILogger.h"  // define ILogger

void doSomethingUseful() 
{
    auto logger = DIContext::resolve<ILogger>();
    logger->log("Desde otro módulo");
}


#endif