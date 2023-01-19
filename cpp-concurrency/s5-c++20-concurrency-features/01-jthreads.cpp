#include  "common.h"

using namespace::std::literals;

void do_some_work() {
    std::cout<<"Do some work \n";
}

void do_something() {
    int counter{0};

    while (counter < 10) {
        std::this_thread::sleep_for(0.2s);
        std::cout<< "This is non-interruptible thread: " << counter << std::endl;
        counter++;
    }
}

void do_something_interruptable(std::stop_token token) {
    int counter{0};

    while (counter < 10) {
        if (token.stop_requested()) {
            return;
        }

        std::this_thread::sleep_for(0.2s);
        std::cout<< "This is interruptible thread: " << counter << std::endl;
        counter++;
    }
}

int main() {
    // std::thread thread1(do_some_work);
    // thread1.join();
    // return 0; If we don't call join or detach we get an error: terminate called without an active exception
 
    std::jthread thread1(do_some_work);
    // no need to call join or detach

    // stop_token is an interruptable argument
    std::jthread interruptabe(do_something_interruptable);
    std::jthread nonInterruptable(do_something);

    std::this_thread::sleep_for(1s);
    interruptabe.request_stop();
    nonInterruptable.request_stop();

    std::cout<<std::endl;
}