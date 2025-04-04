#ifndef CONTAINER_H
#define CONTAINER_H

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <any>
#include <stdexcept>

enum class Scope {
    Singleton,
    Transient
};

class Container {
public:
    template<typename T>
    void registerFactory(std::function<std::shared_ptr<T>(Container&)> factory, Scope scope = Scope::Singleton) {
        const std::type_index index = typeid(T);
        factories[index] = [factory](Container& c) -> std::any {
            return factory(c);
        };
        scopes[index] = scope;
    }

    template<typename T>
    std::shared_ptr<T> resolve() {
        const std::type_index index = typeid(T);

        // Singleton reuse
        if (scopes[index] == Scope::Singleton) {
            auto it = instances.find(index);
            if (it != instances.end()) {
                return std::any_cast<std::shared_ptr<T>>(it->second);
            }
        }

        auto fit = factories.find(index);
        if (fit == factories.end()) {
            throw std::runtime_error("No hay fábrica registrada para el tipo solicitado");
        }

        auto instance = std::any_cast<std::shared_ptr<T>>(fit->second(*this));
        
        if (scopes[index] == Scope::Singleton) {
            instances[index] = instance;
        }

        return instance;
    }

private:
    std::unordered_map<std::type_index, std::function<std::any(Container&)>> factories;
    std::unordered_map<std::type_index, std::any> instances;
    std::unordered_map<std::type_index, Scope> scopes;
};
#endif // CONTAINER_H