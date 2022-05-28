#pragma once

#include <thread>
#include <vector>

class Thread_Guard {
    std::thread &t;

   public:
    // Explicit ensures no implicit type conversion happens for the arguments.
    explicit Thread_Guard(std::thread &&_t) : t(_t) {}
    explicit Thread_Guard(std::thread &_t) : t(_t) {}

    ~Thread_Guard() {
        if (t.joinable()) {
            t.join();
        }
    }

    // Deleting the copy constructor and assignment operator
    // because we don't want it to copy from one to another
    Thread_Guard(Thread_Guard &) = delete;
    Thread_Guard &operator=(Thread_Guard &) = delete;
};