#include "DIConfigurator.h"
#include "ILogger.h"
#include "ConsoleLogger.h"
#include "Application.h"

ConfigDependenciesResult configureDependencies() 
{
    DIConfigurator::registerFactory<ILogger>([](Container&) 
    {
        return std::make_shared<ConsoleLogger>();
    }, 
    Scope::Singleton);

    DIConfigurator::registerFactory<IApplication>([](Container& c) 
    {
        return std::make_shared<Application>(c.resolve<ILogger>());
    }, 
    Scope::Transient);

    // TODO: Add more dependencies as needed

    // TODO: Add logic en case of failure
    return ConfigDependenciesResult::Success;
}