#include "common.h"

struct X {
    int i;
    std::string s;
};

std::atomic<X*> p;
std::atomic<int> a;

void create_x() {
    X* x = new X;
    x->i = 42;
    x->s = "hello";

    a.store(20, std::memory_order_relaxed);
    p.store(x, std::memory_order_release);
}

void use_x() {
    X* x;
    // The following will ensure that this thread see every write that was done by create_x
    // That is we're gauranteed to see thte updated value of a and p
    // while(!(x = p.load(std::memory_order_acquire)));

    // The following will ensure that this thread see the write to p done by create_x
    // There's however no gaurantee that it will see the updated value of a
    while (!(x = p.load(std::memory_order_consume)))
        ;
    assert(x->i == 42);
    assert(x->s == "hello");

    // This may fail
    assert(a.load(std::memory_order_relaxed) == 20);
}

void run_code() {
    {
        std::thread a(create_x);
        std::thread b(use_x);
        a.join();
        b.join();
    }
}

int main() {
    run_code();
}