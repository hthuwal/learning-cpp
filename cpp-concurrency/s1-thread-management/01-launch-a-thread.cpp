#include <iostream>
#include <thread>

void callable_function() {
    std::cout << "Hello from callable function thread. Thread Id: " << std::this_thread::get_id() <<"\n";
}

class callable_class {
   public:
    void operator()() {
        std::cout << "Hello from operator method of callable_class thread. Thread Id: " << std::this_thread::get_id() <<"\n";
    }
};

void run() {
    std::thread thread1(callable_function);
    callable_class callable_obj;
    std::thread thread2(callable_obj);
    std::thread thread3([]() -> void {
        std::cout << "Hello from lambda callable thread. Thread Id: " << std::this_thread::get_id() <<"\n";
    });

    thread1.join();  // forces the main thread to wait for the thread to exit
    thread2.join();
    thread3.join();
    std::cout << "Hello from main. Thread Id: " << std::this_thread::get_id() <<"\n";
}

int main() {
    run();
}