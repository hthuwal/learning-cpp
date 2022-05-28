/*
1: Cleaning, Cleaner does this reprsented by a function
2: Full Speed Ahead, Engine crew does this represented by a function
3: Stop the enginge, Engine crew does this represented by a function
100: Exit
default: Invalid Orders

Captain does not wait for cleaning. Captain has to wait for engine
Captain is main thread
*/

#include <iostream>
#include <thread>

#include "thread_guard.h"

void full_sleep_ahead() {
    printf("Full sleep ahead called.\n");
    fflush(stdout);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    printf("\nFull speed ahead finished after 5 seconds.\nEnter Command: ");
    fflush(stdout);
}

void stop_the_engine() {
    printf("Stop the engine called.\n");
    fflush(stdout);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    printf("\nStop the engine finished after 5 seconds.\nEnter Command: ");
    fflush(stdout);
}

void clean() {
    printf("Cleaner called.\nEnter Command: ");
    fflush(stdout);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    printf("\nCleaner Finished after 5 seconds. \nEnter Command: ");
    fflush(stdout);
}

void run() {
    int command;
    printf("\nEnter Command: ");
    fflush(stdout);
    while (std::cin >> command) {
        if (command == 100) {
            break;
        } else if (command == 1) {
            std::thread ct(clean);
            ct.detach();
        } else if (command == 2) {
            std::thread fst(full_sleep_ahead);
            Thread_Guard tg(fst);
            // Thread joniing is done when we go out of if, thread_guard's destructor is called
            // which does the joining.
        } else if (command == 3) {
            std::thread set(stop_the_engine);
            Thread_Guard tg(set);
        } else {
            std::cout << "Invalid Command"
                      << "\nEnter Command: ";
        }
    }
}

int main() {
    run();
}