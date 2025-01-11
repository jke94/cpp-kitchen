class Fuel
{
    private:
};

class Car
{
private:

public:

    Car(){}
    void before_start(Fuel *fuel)
    {
        // Logic
    }
};

int main()
{
    Car car;
    Fuel* fuel = new Fuel();

    car.before_start(fuel);

    delete fuel;

    return 0;
}