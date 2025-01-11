class Engine
{

};

class Car
{
    public:
        void start_engine(Engine* engine){}
};

int main()
{
    Engine* engine = new Engine();
    Car car;
    car.start_engine(engine);
    
    return 0;
}