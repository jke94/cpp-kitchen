#include <iostream>

template<typename T>
class Interface 
{
public:
    virtual T process(T data) = 0;
};

class ConcreteClass : public Interface<int> 
{
public:

    int process(int data) override 
    {
        // Implementation for Interface<int>
        return data * 2;
    }
};

int main()
{
    ConcreteClass *concrete_class = new ConcreteClass();

    std::cout << concrete_class->process(1994) << std::endl;

    delete concrete_class;

    return 0;
}
