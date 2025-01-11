#include <iostream>
#include <algorithm>

class Target
{
    public:
        virtual ~Target() = default;

        virtual std::string Request() const 
        {
            return "Target: The default target's behavior.";
        }
};

class Adaptee 
{
    public:
        std::string SpecificRequest() const 
        { 
            return ".eetpadA eht fo roivaheb laicepS";
        }
};

class Adapter : public Target 
{
    private:
        Adaptee *adaptee_;

    public:
        Adapter(Adaptee *adaptee) : adaptee_(adaptee) {}

        std::string Request() const override 
        {
            std::string to_reverse = this->adaptee_->SpecificRequest();

            std::reverse(to_reverse.begin(), to_reverse.end());
            
            return "Adapter: (TRANSLATED) " + to_reverse;
        }
};

void ClientCode(const Target *target) 
{
    std::cout << target->Request() << std::endl;
}

int main() 
{
    std::cout << "Client: I can work just fine with the Target objects:\n";
    Target *target = new Target;
    ClientCode(target);
    std::cout << std::endl;
    Adaptee *adaptee = new Adaptee;
    std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
    std::cout << "Adaptee: " << adaptee->SpecificRequest() << std::endl;
    std::cout << std::endl;
    std::cout << "Client: But I can work with it via the Adapter:" << std::endl;
    Adapter *adapter = new Adapter(adaptee);
    ClientCode(adapter);

    delete target;
    delete adaptee;
    delete adapter;

  return 0;
}