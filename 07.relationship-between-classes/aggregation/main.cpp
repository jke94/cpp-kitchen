/**
 *  Summary: Relationship between classes: aggregation (with C++ example).
 *  GitHub author: https://github.com/jke94
 *  Date: 2023, November.
 * 
 *  How to compile:
 *      
 *      $ g++ main.cpp -std=c++20 -o main
*/

class Battery 
{
    public:
        Battery(){};
        ~Battery(){};
};

class Simcard
{
    public:
        Simcard(){};
        ~Simcard(){};
};

class Memorycard
{
    public:
        Memorycard(){};
        ~Memorycard(){};
};

class Mobile
{
    private:

        Battery* battery_ = nullptr;
        Simcard* simcard_ = nullptr;
        Memorycard* memorycard_ = nullptr;

    public:

        Mobile(Battery* battery)
        {
            battery_ = battery;     // Aggregation in constructor level.
        }
        ~Mobile()
        {
            delete battery_;
            delete simcard_;
            delete memorycard_;
        };

        void initialite_memory(Simcard* simcard, Memorycard* memorycard)
        {
            simcard_ = simcard;             // Aggregation in method level.
            memorycard_ = memorycard;       // Aggregation in method level.
        }
};

int main()
{
    Battery* batery = new Battery();
    Simcard* simcard = new Simcard();
    Memorycard* memorycard = new Memorycard();

    Mobile* mobile = new Mobile(batery);                // Objects inyection in contructor as parameter.
    mobile->initialite_memory(simcard, memorycard);     // Objects inyection in method as parameter.
    
    delete mobile;
    
    return 0;
}