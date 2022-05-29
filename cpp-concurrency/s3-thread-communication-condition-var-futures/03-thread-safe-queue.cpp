#include <condition_variable>
#include <iostream>
#include <queue>

template <typename T>
class thread_safe_queue {
   public:
    thread_safe_queue();
    thread_safe_queue(const thread_safe_queue& other_queue);

    void push(const T& value) {
        std::lock_guard<std::mutex> lg(m);
        queue.push(std::make_shared<T>(value));
        cv.notify_one();  // If there are any waiting threads in wait_pop, this will cause them to wake up and check condition again
    }

    // See stack and section 2 to understand why we taking a ref
    void pop(T& ref) {
        std::lock_guard<std::mutex> lg(m);
        if (queue.empty()) {
            throw std::runtime_error("queue is empty");
        }

        ref = *(queue.front().get());
        queue.pop();
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lg(m);
        if (queue.empty()) {
            return std::shared_ptr<T>();
        }

        std::shared_ptr<T> ref(queue.front());
        queue.pop();
        return ref;
    }

    bool empty() {
        std::lock_guard<std::mutex> lg(m);
        return queue.empty();
    }

    size_t size() {
        std::lock_guard<std::mutex> lg(m);
        return queue.size();
    }

    // These methods will wait for an element to be available in the queue
    void wait_pop(T& ref) {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this] {
            return !queue.empty();
        });
        ref = *(queue.front().get());
        queue.pop();
    }
    
    std::shared_pt<T> wait_pop() {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this] {
            return !queue.empty();
        });
        std::shared_ptr<T> ref = queue.front();
        queue.pop();
        return ref;
    }

   private:
    std::mutex m;
    std::condition_variable cv;
    std::queue<std::shared_ptr<T>> queue;
};