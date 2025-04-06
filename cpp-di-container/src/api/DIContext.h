#ifndef DI_CONTEXT_H
#define DI_CONTEXT_H

#include "Container.h"

/**
 * @brief Dependency Injection Context.
 * @details This class provides a singleton instance of the DI container and allows for the registration and resolution of dependencies.
 */
class DIConfigurator;

/**
 * @brief Dependency Injection Context.
 * @details This class provides a singleton instance of the DI container and allows for the registration and resolution of dependencies.
 */
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

    // Only DIConfigurator can register factories
    friend class DIConfigurator;

    template<typename T>
    static void registerFactory(std::function<std::shared_ptr<T>(Container&)> factory, Scope scope = Scope::Singleton) 
    {
        instance().registerFactory<T>(std::move(factory), scope);
    }
};

/**
 * @brief Application configuration structure.
 * @details This structure holds the configuration settings for the application.
 */
struct AppConfig 
{
    std::string logFilename;
    int port;
    std::string dbConnectionString;
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
ConfigDependenciesResult configureDependencies(AppConfig& appConfig);

#endif // DI_CONTEXT_H