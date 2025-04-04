#ifndef DI_CONTEXT_H
#define DI_CONTEXT_H

#include "Container.h"

class DIConfigurator;

class DIContext 
{
public:
    static Container& instance() 
    {
        static Container container;
        return container;
    }

    template<typename T>
    static std::shared_ptr<T> resolve() 
    {
        return instance().resolve<T>();
    }

private:
    // Sólo DIConfigurator puede registrar
    friend class DIConfigurator;

    template<typename T>
    static void registerFactory(std::function<std::shared_ptr<T>(Container&)> factory, Scope scope = Scope::Singleton) 
    {
        instance().registerFactory<T>(std::move(factory), scope);
    }
};

/**
 * @brief Result of the configuration dependencies process.
 * @details This enum is used to indicate the success or failure of the configuration process.
 */
enum class ConfigDependenciesResult 
{
    Success,
    Failure
};

/**
 * @brief Configures the DI container with the necessary dependencies.
 * @details This function registers the factories for the dependencies in the DI container.
 */
ConfigDependenciesResult configureDependencies();

#endif // DI_CONTEXT_H