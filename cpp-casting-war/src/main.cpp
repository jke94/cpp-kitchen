#include <ctime>
#include <iostream>
#include <memory>
#include <string>

namespace cppCastingWar
{
    // ------------------------------------------------------------------------
    // API definition with the client.
    // ------------------------------------------------------------------------

    class IDevice
    {
    public:
        virtual ~IDevice() = default;
        // It´s the base class for all devices. Never define nothing here :). The casting make the magic.
    protected:
        IDevice() = default;
        IDevice(const IDevice&) = default;
        IDevice& operator=(const IDevice&) = default;
        IDevice(IDevice&&) = default;
        IDevice& operator=(IDevice&&) = default;
    };

    class IPrimiumDevice : public IDevice
    {
    public:
        virtual ~IPrimiumDevice() = default;
        virtual void specialFeature() = 0;
        virtual bool setAuthentication(std::string& user, std::string& passwordA, std::string& passwordB) = 0;

    protected:
        IPrimiumDevice() = default;
        IPrimiumDevice(const IPrimiumDevice&) = default;
        IPrimiumDevice& operator=(const IPrimiumDevice&) = default;
        IPrimiumDevice(IPrimiumDevice&&) = default;
        IPrimiumDevice& operator=(IPrimiumDevice&&) = default;
    };

    class IUltraDevice : public IDevice
    {
    public:
        virtual ~IUltraDevice() = default;
        virtual void ultraFeature() = 0;
        virtual bool setAuthentication(const std::string token) = 0;

    protected:
        IUltraDevice() = default;
        IUltraDevice(const IUltraDevice&) = default;
        IUltraDevice& operator=(const IUltraDevice&) = default;
        IUltraDevice(IUltraDevice&&) = default;
    };

    // ------------------------------------------------------------------------
    // Concrete implementations of the API.
    // ------------------------------------------------------------------------

    class PrimiumDevice final : public IPrimiumDevice
    {
    public:
        PrimiumDevice() = default;
        ~PrimiumDevice() override = default;
        void specialFeature() override;
        bool setAuthentication(std::string& user, std::string& passwordA, std::string& passwordB) override;
    };

    void PrimiumDevice::specialFeature()
    {
        std::cout << "PrimiumDevice: Executing special feature." << std::endl;
    }

    bool PrimiumDevice::setAuthentication(std::string& user, std::string& passwordA, std::string& passwordB)
    {
        // Simulate authentication logic.
        if(user.empty() || passwordA.empty() || passwordB.empty())
        {
            return false;
        }
        std::cout << "PrimiumDevice: Authentication successful for user: " << user << std::endl;
        return true;
    }

    class UltraDevice final : public IUltraDevice
    {
    public:
        UltraDevice() = default;
        ~UltraDevice() override = default;
        void ultraFeature() override;
        bool setAuthentication(const std::string token) override;
    };

    void UltraDevice::ultraFeature()
    {
        std::cout << "UltraDevice: Executing special ultra feature." << std::endl;
    }

    bool UltraDevice::setAuthentication(const std::string token)
    {
        // Simulate authentication logic.
        if(token.empty())
        {
            return false;
        }
        std::cout << "UltraDevice: Authentication successful with token: " << token << std::endl;
        return true;
    }

    std::shared_ptr<IDevice> createDevice()
    {
        // Simulate a random device creation process.
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        const int randomValue = rand() % 100; // Random value between 0 and 99
        std::cout << "Random value generated: " << randomValue << std::endl;

        std::shared_ptr<IDevice> result;
        if(randomValue < 60) // 60 % probability
        {
            result = std::make_shared<UltraDevice>();
        }
        else
        {
            result = std::make_shared<PrimiumDevice>();
        }

        return result;
    }

}; // namespace cppCastingWar

using namespace cppCastingWar;

int main(int argc, char* argv[])
{
    std::shared_ptr<IDevice> device = createDevice();

    if(!device) 
    {
        // Some (development) new concrete class that is not derived from IDevice. The client does not know about it.
        return -1;
    }

    if(std::shared_ptr<IPrimiumDevice> primiumDevice = std::dynamic_pointer_cast<IPrimiumDevice>(device))
    {
        primiumDevice->specialFeature();
        std::string user = "user123";
        std::string passwordA = "passwordA";
        std::string passwordB = "passwordB";
        
        if(primiumDevice->setAuthentication(user, passwordA, passwordB))
        {
            std::cout << "PrimiumDevice authentication successful." << std::endl;
        }
        else
        {
            std::cout << "PrimiumDevice authentication failed." << std::endl;
        }
    }
    else if(std::shared_ptr<IUltraDevice> ultraDevice = std::dynamic_pointer_cast<IUltraDevice>(device))
    {
        ultraDevice->ultraFeature();

        if(ultraDevice->setAuthentication("token123"))
        {
            std::cout << "UltraDevice authentication successful." << std::endl;
        }
        else
        {
            std::cout << "UltraDevice authentication failed." << std::endl;
        }
    }
    else
    {
        std::cout << "Unknown device type." << std::endl;
    }

    return 0;
}