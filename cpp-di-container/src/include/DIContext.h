#ifndef DI_CONTEXT_H
#define DI_CONTEXT_H

#include "Container.h"

class DIContext {
public:
    static Container& instance() {
        static Container container;
        return container;
    }

    template<typename T>
    static std::shared_ptr<T> resolve() {
        return instance().resolve<T>();
    }

    template<typename T>
    static void registerFactory(std::function<std::shared_ptr<T>(Container&)> factory, Scope scope = Scope::Singleton) {
        instance().registerFactory<T>(std::move(factory), scope);
    }
};

#endif // DI_CONTEXT_H