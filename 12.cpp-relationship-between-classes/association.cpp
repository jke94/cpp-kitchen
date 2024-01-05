class Engine
{

};

class Car 
{
    private:
        Engine* _engine;
    public:
        Car(Engine* engine) : _engine (engine){}
};

int main()
{
    Car car(new Engine());
    
    return 0;
}