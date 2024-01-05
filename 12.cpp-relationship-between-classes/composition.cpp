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
        Car(Engine* engine) : _engine(engine){}
        void start_engine(){}
};

int main()
{
    Engine* engine = new Engine();
    Car car(engine);
    car.start_engine();
    
    return 0;
}