#include <iostream>
#include <string>

class Automobile
{
public:
    void start()
    {
        std::cout << "I am a car." << std::endl;
    }
};

class Taxi : public Automobile
{
public:
    void turn_on()
    {
        std::cout << "I am a taxi, turning on engine." << std::endl;
    }
};

class TucTuc : public Automobile
{
public:
    void turn_on()
    {
        std::cout << "I am a tuc-tuc, turning on engine." << std::endl;
    }
};

int main()
{
    Taxi taxi;
    taxi.start();
    taxi.turn_on();

    TucTuc tuctuc;
    tuctuc.start();
    tuctuc.turn_on();

    return 0;
}