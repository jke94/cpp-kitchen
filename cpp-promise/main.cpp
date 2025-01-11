#include <iostream>
#include <thread>
#include <future>

void modify_message(std::promise<std::string> prms, std::string message)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(4000)); // simulate work
    std::string msg_modified = message + " has been modified"; 
    
    prms.set_value(msg_modified);
}

int main()
{
    // define message
    std::string msg_to_thread = "My Message";

    // create promise and future
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();

    // start thread and pass promise as argument
    std::thread t(modify_message, std::move(promise), msg_to_thread);

    // print original message to console
    std::cout << "Original message from main(): " << msg_to_thread << std::endl;

    // retrieve modified message via future and print to console
    std::string msg_from_thread = future.get();
    std::cout << "Modified message from thread(): " << msg_from_thread << std::endl;

    // thread barrier
    t.join();

    return 0;
}