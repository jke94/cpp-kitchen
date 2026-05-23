#include <iostream>

class IVehicle
{
public:

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes
     */
    virtual ~IVehicle() = default;
protected:
    IVehicle() = default;
    IVehicle(const IVehicle&) = default;
    IVehicle& operator=(const IVehicle&) = default;
};

class Car : public IVehicle
{
public:
    ~Car() override
    {
        std::cout << "Car destructor called" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    IVehicle* vehicle = new Car();
    IVehicle* vehicle2 = vehicle;

    std::cout << "Vehicle pointer: " << vehicle << std::endl;
    std::cout << "Vehicle2 pointer: " << vehicle2 << std::endl;

    delete vehicle; // This will call the Car destructor due to the virtual destructor in IVehicle

    return 0;
}