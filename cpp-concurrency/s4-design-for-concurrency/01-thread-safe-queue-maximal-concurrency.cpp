#include <condition_variable>
#include <memory>
#include <thread>

template <typename T>
class sequential_queue {
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;

        node(T _data) {
            data = std::make_shared<T>(std::move(_data));
        }
    };

    std::unique_ptr<node> head;
    node* tail;
    std::mutex head_mutex;
    std::mutex tail_mutex;

   public:
    sequential_queue() : head(new node), tail(head.get());  // We create a dummy node to which both tail always points to
    void push(T value) {
        std::unique_ptr<node> new_dummy_node(new node);
        const node* new_tail = new_dummy_node.get();

        // Limiting the scope of the lock
        {
            std::lock_guard<std::mutex> lg(tail_mutex);
            
            // change current dummy nodes data value
            tail->data = std::make_shared<T>(std::move(value));
            tail->next = std::move(new_dummy_node);
            tail = new_tail;
        }
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lg(head_mutex);
        // Both point to dummy, no element
        {
            std::lock_guard<std::mutex> lg(tail_mutex);
            if (head.get() == tail) {
                return std::shared_ptr<T>();
            }
        }
        const std::shared_ptr<T> res(head->data);
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }
};