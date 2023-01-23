#include "common.h"

template <typename T>
class LockFreeStack {
   private:
    struct node;

    struct node_wrappper {
        int external_count;
        node* ptr;
    }

    struct node {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;

        node_wrappper* next;
        node(T const& data_) : data(std::make_shared<T>(data_)), internal_count(0) {
        }
    };

    std::atomic<node_wrappper*> head;

   public:
    void push(T const& data_) {
        node_wrappper new_node;
        new_node.ptr = new node(data_);
        new_node.external_count = 1;
        new_node.ptr->next = head.load();

        /**
         * If head hasn't changed by another thread
         * Compare exchange (haed == new_node->next) will be true, head will be replaced with new_node
         *
         * If head has been changed by another thread to say head2
         * Compare exchange (head == new_node->next) becomes false, new_node->next will be updated to head2
         *
         * Refer compare exchange section in section 6's readme.
         */
        while (!head.compare_exchange_weak(new_node.ptr->next, new_node))
            ;
    }

    // I don't trust this
    // Read C++ concurrency in action to understand this
    std::shared_ptr<T> pop() {
        node_wrappper old_head = head.load();

        while (true) {
            increment_head_ref_count(old_head);

            node* const ptr = old_head.ptr;
            if (!ptr) {
                return std::shared_ptr<T>();
            }

            if (head.compare_exchange_strong(old_head, ptr->next)) {
                std::shared_ptr<T> res;
                res.swap(ptr->data);

                int const current_external_count = old_head.external_count - 2;

                if (ptr->internal_count.fetch_add(current_external_count) != 0) {
                    delete ptr;
                }
                return res;
            } else if (ptr->internal_count.fetch_sub(1) == 1) {
                delete ptr;
            }
        }
    }

    void increment_head_ref_count(node_wrappper& old_head) {
        node_wrappper new_head;
        do {
            new_head = old_head;
            ++new_head.external_count;
        } while (!head.compare_exchange_strong(old_head, new_head));
        old_head.external_count = new_head.external_count;
    }
};