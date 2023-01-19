#include "common.h"

class interrupt_flag {
    bool local_flag;

   public:
    void set() {
        local_flag = true;
    }

    bool is_Set() {
        return local_flag;
    }
};

// New instance in each thread that is spawned
thread_local interrupt_flag this_thread_flag;

class jthread_local {
    std::thread _thread;
    interrupt_flag* flag;

   public:
    template <typename FunctionType>
    jthread_local(FunctionType f) {
        std::promise<interrupt_flag*> p;
        _thread = std::thread([f, &p] {
            // Return the thread's thread local flag
            p.set_value(&this_thread_flag);
            f();
        });
        flag = p.get_future().get();
    }

    void interrupt() {
        flag->set();
    }

    ~jthread_local() {
        if (_thread.joinable()) {
            _thread.join();
        }
    }
};

using namespace ::std::literals;

void do_something() {
    int counter{0};

    while (counter < 10) {
        std::this_thread::sleep_for(0.2s);
        std::cout << "This is non-interruptible thread: " << counter << std::endl;
        counter++;
    }
}

bool interrupt_point() {
    if (this_thread_flag.is_Set()) {
        return true;
    }
    return false;
}

void do_something_interruptable() {
    int counter{0};

    while (counter < 10) {
        if (interrupt_point()) {
            return;
        }

        std::this_thread::sleep_for(0.2s);
        std::cout << "This is interruptible thread: " << counter << std::endl;
        counter++;
    }
}

int main() {
    // stop_token is an interruptable argument
    jthread_local interruptabe(do_something_interruptable);
    jthread_local nonInterruptable(do_something);

    std::this_thread::sleep_for(1s);
    interruptabe.interrupt();
    nonInterruptable.interrupt();

    std::cout << std::endl;
}