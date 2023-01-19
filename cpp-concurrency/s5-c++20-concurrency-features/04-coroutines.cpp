#include <cassert>
#include <coroutine>

#include "common.h"

class Resumable {
   public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    Resumable(handle_type handle) : handle_(handle) {
        assert(handle);
    };

    Resumable(Resumable&) = delete;
    Resumable(Resumable&&) = delete;

    bool resume() {
        if (not handle_.done()) {
            handle_.resume();
        }
        return not handle_.done();
    }

   private:
    handle_type handle_;
};

struct Resumable::promise_type {
    using handle_type = std::coroutine_handle<promise_type>;

    // Mandatory Methods

    // Returns handle
    auto get_return_object() {
        return handle_type::from_promise(*this);
    }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}

    void unhandled_exception() {
        std::terminate();
    }
};

Resumable foo() {
    std::cout << "A" << std::endl;
    // Suspends the execution here indefinitely until resume is called
    co_await std::suspend_always();
    std::cout << "B" << std::endl;
    // Suspends the execution here indefinitely until resume is called
    co_await std::suspend_always();
    std::cout << "C" << std::endl;
}

int main() {
    // At this point the coroutine is suspended to begin with 
    Resumable res1 = foo();
    res1.resume(); // Resume from initial suspend
    res1.resume(); // Resume from first co_await
    res1.resume(); // Resume from second co_await
}