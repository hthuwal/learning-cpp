#include<iostream>
#include<stack>
#include<mutex>
#include<memory>

/**
Storing the shared_ptr to element instead of the element because returning value on pop will lead to memory allocation
and memory allocation may lead to issues if there is no space left.
- Returning shared ptr will solve this.
- Another option is to have the caller allocate memory and pass us reference to it and we'll update it
**/

template<typename T>
class trivial_thread_safe_stack {

    std::stack<std::shared_ptr<T>> stk;
    std::mutex m;
    public:
    void push(T ele) {
        std::lock_guard<std::mutex> lg(m);
        stk.push(std::make_shared<T>(ele));
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lg(m);
        if (stk.empty()) {
            throw std::runtime_error("stack is empty");
        }
        std::shared_ptr<T> res(stk.top());
        stk.pop();
        return res;
    }

    void pop(T& value) {
        std::lock_guard<std::mutex> lg(m);
        if (stk.empty()) {
            throw std::runtime_error("stack is empty");
        }
        value = *(stk.top().get());
        stk.pop();
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