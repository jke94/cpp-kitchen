#include "di_config.h"

#include "DIContext.h"
#include "ILogger.h"
#include "ConsoleLogger.h"
#include "Application.h"

void configureDependencies() 
{
    DIContext::registerFactory<ILogger>([](Container&) 
    {
        return std::make_shared<ConsoleLogger>();
    }, 
    Scope::Singleton); // 👈 Aquí especificamos el scope

    DIContext::registerFactory<IApplication>([](Container& c) 
    {
        return std::make_shared<Application>(c.resolve<ILogger>());
    }, 
    Scope::Transient);  // 👈 Aquí especificamos el scope
}