#include <iostream>
#include <string>
#include <memory>
#include <sstream>

namespace cppBuilderPattern
{
    class IDevice
    {
    public:
        virtual ~IDevice() = default;
        virtual std::string toString() = 0;
        
    protected:
        IDevice() = default;
        IDevice(const IDevice&) = delete;
        IDevice& operator=(const IDevice&) = delete;
        IDevice(IDevice&&) = delete;
        IDevice& operator=(IDevice&&) = delete;        
    };

    class Device final : public IDevice
    {
    public:
        Device() = default;
        ~Device() override = default;
        void setName(const std::string &name);
        void setPrice(double price);
        void setColor(const std::string &color);
        std::string toString() override;

    private:
        std::string name;
        double price;
        std::string color;
    };

    void Device::setName(const std::string &name) 
    { 
        this->name = name;
    }

    void Device::setPrice(double price) 
    {
        this->price = price; 
    }

    void Device::setColor(const std::string &color) 
    { 
        this->color = color; 
    }

    std::string Device::toString()
    {
        std::ostringstream oss;
        oss << "Device [Name: " << name 
            << ", Price: " << price 
            << ", Color: " << color 
            << "]";
        return oss.str();
    }

    // Builder interface
    class IDeviceBuilder
    {
    public:
        virtual ~IDeviceBuilder() = default;
        virtual void reset() = 0;
        virtual void buildName() = 0;
        virtual void buildPrice() = 0;
        virtual void buildColor() = 0;
        virtual std::unique_ptr<IDevice> getResult() = 0;

    protected:
        IDeviceBuilder() = default;
        IDeviceBuilder(const IDeviceBuilder&) = delete;
        IDeviceBuilder& operator=(const IDeviceBuilder&) = delete;
        IDeviceBuilder(IDeviceBuilder&&) = delete;
        IDeviceBuilder& operator=(IDeviceBuilder&&) = delete;

    };

    // Concrete builder
    class DeviceBuilder : public IDeviceBuilder
    {
    public:
        DeviceBuilder() { reset(); }

        void reset() override
        {
            device = std::make_unique<Device>();
        }

        void buildName() override
        {
            device->setName(name);
        }

        void buildPrice() override
        {
            device->setPrice(price);
        }

        void buildColor() override
        {
            device->setColor(color);
        }

        std::unique_ptr<IDevice> getResult() override
        {
            return std::move(device);
        }

        // Setters to configure what to build
        void setName(const std::string& n) { name = n; }
        void setPrice(double p) { price = p; }
        void setColor(const std::string& c) { color = c; }

    private:
        std::unique_ptr<Device> device;
        std::string name;
        double price;
        std::string color;
    };

    // Director
    class DeviceDirector
    {
    public:
        void setBuilder(DeviceBuilder* b) { builder = b; }

        void constructSmartphone()
        {
            builder->reset();
            builder->setName("Smartphone");
            builder->setPrice(699.99);
            builder->setColor("Black");

            builder->buildName();
            builder->buildPrice();
            builder->buildColor();
        }

        void constructLaptop()
        {
            builder->reset();
            builder->setName("Laptop");
            builder->setPrice(1299.99);
            builder->setColor("Silver");

            builder->buildName();
            builder->buildPrice();
            builder->buildColor();
        }

    private:
        DeviceBuilder* builder;
    };

} // namespace cppBuilderPattern

using namespace cppBuilderPattern;

int main()
{
    DeviceBuilder builder;
    DeviceDirector director;

    director.setBuilder(&builder);

    director.constructSmartphone();
    std::unique_ptr<IDevice> smartphone = builder.getResult();
    std::cout << smartphone->toString() << std::endl;

    director.constructLaptop();
    std::unique_ptr<IDevice> laptop = builder.getResult();
    std::cout << laptop->toString() << std::endl;

    return 0;
}
