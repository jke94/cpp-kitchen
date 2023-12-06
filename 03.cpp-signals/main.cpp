// CPP Program to demonstrate the signal() function
#include <csignal>
#include <iostream>
  
void signal_handler(int signal_num)
{
    std::cout << "The interrupt signal is (" << signal_num << ")." << std::endl;;
  
    // It terminates the  program
    exit(signal_num);
}
  
int main()
{
    // register signal SIGABRT and signal handler
    signal(SIGABRT , signal_handler);
  
    while (true)
    {
        std::cout << "Hello GeeksforGeeks..." << std::endl;
    }
    return 0;
}