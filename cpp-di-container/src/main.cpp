#include <iostream>

#include "DIContext.h"
#include "IApplication.h"
#include "ILogger.h"

int main() 
{
    ConfigDependenciesResult configDependenciesResultType = configureDependencies();
    
    if (configDependenciesResultType != ConfigDependenciesResult::Success) 
    {
        std::cerr << "Error configuring dependencies" << std::endl;
        return -1;
    }
    std::cout << "Dependencies configured successfully" << std::endl;

    auto app1 = DIContext::resolve<IApplication>();
    auto app2 = DIContext::resolve<IApplication>();

    app1->run();
    app2->run(); // Debe crear una nueva instancia si es transient

    auto logger = DIContext::resolve<ILogger>();
    logger->log("Usando logger compartido desde main");

    return 0;
}
