#include <vector>

class Engine
{

};

class Car
{
    private:
        Engine* _engine;

    public:
        Car(){}
        ~Car(){ delete _engine; }
        void add_engine(Engine* engine)
        {
            _engine = engine;
        }
};

int main()
{
    Engine* renault_engine = new Engine();
    Engine* mercedes_engine = new Engine();

    Car mercedes;
    mercedes.add_engine(renault_engine);

    // more logic... and engine change!

    mercedes.add_engine(mercedes_engine);

    delete renault_engine;
    delete mercedes_engine;
    
    return 0;
}