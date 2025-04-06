#include <iostream>

#include "DIContext.h"
#include "IApplication.h"

AppConfig parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[]) 
{
    AppConfig appConfig = parseArgs(argc, argv);

    ConfigDependenciesResult configDependenciesResultType = configureDependencies(appConfig);
    
    if (configDependenciesResultType != ConfigDependenciesResult::Success) 
    {
        std::cerr << "Error configuring dependencies" << std::endl;
        return -1;
    }

    std::cout << "Dependencies configured successfully" << std::endl;

    // Play with the DI container

    auto app1 = DIContext::resolve<IApplication>();
    auto app2 = DIContext::resolve<IApplication>();

    app1->run();
    app2->run(); // Should be different instance as app1

    return 0;
}

AppConfig parseArgs(int argc, char* argv[])
{
    AppConfig appConfig;

    // TODO: Implement parseArgs to parse command line arguments

    appConfig.logFilename = "app.log";
    appConfig.port = 8080;
    appConfig.dbConnectionString = "localhost:5432/mydb";
    
    return appConfig;
}
