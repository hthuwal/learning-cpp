#include<iostream>
#include<stack>
#include<mutex>

/**
 * We ensured thread safety but limited true parallelism because nothing can be done in parallel.
 **/

template<typename T>
class trivial_thread_safe_stack {
    std::stack<T> stk;
    std::mutex m;
    public:
    void push(T ele) {
        std::lock_guard<std::mutex> lg(m);
        stk.push(ele);
    }

    void pop() {
        std::lock_guard<std::mutex> lg(m);
        stk.pop();
    }

    T& top() {
        std::lock_guard<std::mutex> lg(m);
        return stk.top();
    }

    int size() {
        std::lock_guard<std::mutex> lg(m);
        return stk.size();
    }

    bool empty() {
        std::lock_guard<std::mutex> lg(m);
        return stk.empty();
    }
};