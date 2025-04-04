#include <iostream>

#include "DIContext.h"

#include "di_config.h"
#include "IApplication.h"
#include "ILogger.h"

int main() 
{
    configureDependencies();

    auto app1 = DIContext::resolve<IApplication>();
    auto app2 = DIContext::resolve<IApplication>();

    app1->run();
    app2->run(); // Debe crear una nueva instancia si es transient

    auto logger = DIContext::resolve<ILogger>();
    logger->log("Usando logger compartido desde main");

    return 0;
}
