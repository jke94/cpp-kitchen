#include <iostream>
#include <memory>
#include <string>

namespace templating
{
    template <typename T>
    class IHandler
    {
    public:
        virtual ~IHandler<T>() = default;
        virtual void handle(T data) = 0;
    };

    class IVehicle
    {
    public:
        virtual ~IVehicle() = default;
        virtual std::string getType() const = 0;
    };

    class Car final : public IVehicle
    {
    public:
        std::string getType() const override
        {
            return "Car";
        }
    };

    class Bike final : public IVehicle
    {
    public:
        std::string getType() const override
        {
            return "Bike";
        }
    };

    class VehicleHandler final : public IHandler<IVehicle*>
    {
    public:
        void handle(IVehicle* vehicle) override
        {
            if(!vehicle)
            {
                return;
            }
            
            std::cout << "Handling a " << vehicle->getType() << ": " << vehicle << std::endl;
        }
    };    

} // namespace templating

using namespace templating;

int main(int argc, char* argv[])
{
    // Example usage of the template function
    std::unique_ptr<IHandler<IVehicle*>> handler = std::make_unique<VehicleHandler>();
    
    std::unique_ptr<IVehicle> car = std::make_unique<Car>();
    handler->handle(car.get());

    std::unique_ptr<IVehicle> bike = std::make_unique<Bike>();
    handler->handle(bike.get());

    return 0;   
}