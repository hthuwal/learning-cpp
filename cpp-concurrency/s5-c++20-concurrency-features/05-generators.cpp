#include <coroutine>

#include "common.h"

/* -------------------------------------------------------------------------- */
/*                            Non Generator Example                           */
/* -------------------------------------------------------------------------- */

std::vector<int> getNumbers(int start, int end, int inc = 1) {
    std::vector<int> nums;
    for (int i = start; i < end; i += inc) {
        nums.push_back(i);
    }
    return std::move(nums);
}

void non_generator_example() {
    const auto numbers = getNumbers(10, 30);

    // Here we only need 10 numbers but has to wait for the all 20 to be generated
    // What if we could generate these numbers as needed
    for (int i = 0; i < 10; i++) {
        std::cout << numbers[i] << " ";
    }

    std::cout << "\n";

    for (int i = 10; i < 20; i++) {
        std::cout << numbers[i] << " ";
    }
}

/* -------------------------------------------------------------------------- */
/*                              Generator Example                             */
/* -------------------------------------------------------------------------- */

template <typename T>
class Generator {
   public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    // Calls implicilty the first time the generator is called
    Generator(handle_type h) : handle(h){};

    ~Generator() {
        if (handle) {
            handle.destroy();
        }
    }

    T getValue() {
        return handle.promise().current_value;
    }

    bool next() {
        handle.resume();
        return not handle.done();
    }

   private:
    handle_type handle;
};

template <typename T>
struct Generator<T>::promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;

    // Mandatory Methods

    // Returns handle
    auto get_return_object() {
        return coro_handle::from_promise(*this);
    }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}

    auto yield_value(const T value) {
        // set cuurent_value to whatever you wanna vield
        current_value = value;
        // block indefinitely
        return std::suspend_always{};
    }

    void unhandled_exception() {
        std::terminate();
    }

    T current_value;
};

Generator<int> getNext(int start, int step = 1) noexcept {
    auto value = start;
    while (true) {
        co_yield value;
        value += step;
    }
}

int main() {
    auto gen = getNext(0);
    for (int i = 0; i <= 10; i++) {
        gen.next();
        std::cout << " " << gen.getValue();
    }

    auto gen2 = getNext(100, -10);
    for (int i = 0; i <= 10; i++) {
        gen2.next();
        std::cout << " " << gen2.getValue();
    }
}