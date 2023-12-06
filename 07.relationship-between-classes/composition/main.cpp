/**
 *  Summary: Relationship between classes: composition (with C++ example).
 *  GitHub author: https://github.com/jke94
 *  Date: 2023, November.
 * 
 *  How to compile:
 *      
 *      $ g++ main.cpp -std=c++20 -o main
*/

class Camera
{
    public:
        Camera(){};
        ~Camera(){};
};

class Message
{
    public:
        Message(){};
        ~Message(){};
};

class Sofwtare
{
    public:
        Sofwtare(){};
        ~Sofwtare(){};
};

class Mobile
{
    private:

        Message* message_ = new Message(); // Composition in attribute level.
        Camera* camera_ = nullptr;
        Sofwtare* software_ = nullptr;

    public:

        Mobile()
        {
            camera_ = new Camera(); // Composition in constructor level.
        }
        ~Mobile()
        {
            delete message_;
            delete camera_;
            delete software_;
        };

        void initialite_software()
        {
            software_ = new Sofwtare(); // Composition in method level.
        }
};

int main()
{
   Mobile* mobile = new Mobile();
   mobile->initialite_software();
   
   delete mobile;
   
   return 0;
}