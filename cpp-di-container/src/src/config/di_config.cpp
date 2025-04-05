#include "DIConfigurator.h"
#include "DIContext.h"
#include "ILogger.h"
#include "ConsoleLogger.h"

#include "IApplication.h"
#include "Application.h"
#include "AlphaService.h"
#include "IService.h"

ConfigDependenciesResult configureDependencies(AppConfig& appConfig) 
{
    DIConfigurator::registerFactory<AppConfig>([&appConfig](Container&) 
    {
        return std::make_shared<AppConfig>(appConfig);
    },
    Scope::Singleton);

    DIConfigurator::registerFactory<ILogger>([](Container& c) 
    {
        std::string filename = c.resolve<AppConfig>()->logFilename;

        return std::make_shared<ConsoleLogger>(filename);
    }, 
    Scope::Singleton);

    DIConfigurator::registerFactory<IApplication>([](Container& c) 
    {
        return std::make_shared<Application>(c.resolve<ILogger>(), c.resolve<IService>());
    }, 
    Scope::Transient);

    DIConfigurator::registerFactory<IService>([](Container& c) 
    {
        return std::make_shared<AlphaService>();
    }, 
    Scope::Singleton);

    // TODO: Add more dependencies as needed

    // TODO: Add logic en case of failure
    return ConfigDependenciesResult::Success;
}