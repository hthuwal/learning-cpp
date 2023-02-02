#pragma once

#include <thread>
#include <vector>

class Thread_Guard {
    std::thread& t;

   public:
    // Explicit ensures no implicit type conversion happens for the arguments.
    explicit Thread_Guard(std::thread&& _t) : t(_t) {}
    explicit Thread_Guard(std::thread& _t) : t(_t) {}

    ~Thread_Guard() {
        if (t.joinable()) {
            t.join();
        }
    }

    // Deleting the copy constructor and assignment operator
    // because we don't want it to copy from one to another
    Thread_Guard(Thread_Guard&) = delete;
    Thread_Guard& operator=(Thread_Guard&) = delete;
};

class Join_Threads {
    std::vector<std::thread>& threads;

   public:
    explicit Join_Threads(std::vector<std::thread>& _threads) : threads(_threads){};
    ~Join_Threads() {
        for (auto& th : threads) {
            if (th.joinable()) {
                th.join();
            }
        }
    }
};

/**
 * Wrapper to denote Any callable with move constructor
 */
class function_wrapper {
    struct impl_base {
        virtual void call() = 0;
        virtual ~impl_base() {}
    };

    template <typename F>
    struct impl_type : impl_base {
        F f;
        impl_type(F&& f_) : f(std::move(f_)) {}
        void call() { f(); }
    };

    std::unique_ptr<impl_base> impl;

   public:
    template <typename F>
    function_wrapper(F&& f) : impl(new impl_type<F>(std::move(f))) {}

    void operator()() { impl->call(); }

    function_wrapper() {}

    function_wrapper(function_wrapper&& other) : impl(std::move(other.impl)) {}

    function_wrapper& operator=(function_wrapper&& other) {
        impl = std::move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper&) = delete;
    function_wrapper(function_wrapper&) = delete;
};