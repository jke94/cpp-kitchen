#ifndef DI_CONFIGURATOR_H
#define DI_CONFIGURATOR_H

#include "DIContext.h"

class DIConfigurator 
{
public:
    template<typename T>
    static void registerFactory(std::function<std::shared_ptr<T>(Container&)> factory, Scope scope = Scope::Singleton) 
    {
        DIContext::registerFactory<T>(std::move(factory), scope);
    }
};

#endif // DI_CONFIGURATOR_H