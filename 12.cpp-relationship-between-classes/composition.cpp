class Engine
{
public:
    void turn_on() 
    {
        // Logic to turn on engine.
    }
};

class Car
{
    private:
        Engine* _engine;
    public:
        Car()
        {
            _engine = new Engine();
        }
        ~Car()
        {
            delete _engine;
        }
        void start_engine()
        {
            _engine->turn_on();
        }
};

int main()
{
    Car car;
    car.start_engine();
    
    return 0;
}