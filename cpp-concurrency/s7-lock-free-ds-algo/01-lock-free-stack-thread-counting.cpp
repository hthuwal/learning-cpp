#include "common.h"

template <typename T>
class LockFreeStack {
   private:
    struct node {
        std::shared_ptr<T> data;
        node* next;
        node(T const& data_) : data(std::make_shared<T>(data_)) {
            next = nullptr;
        }
    };

    std::atomic<node*> head;
    std::atomic<int> threads_in_pop;
    std::atomic<node*> to_be_deleted;

    void try_reclaim(node* old_head) {
        if (threads_in_pop == 1) {
            delete old_head;

            node* claimed_list = to_be_deleted.exchange(nullptr);

            int numThreadsLeft = threads_in_pop--;
            if (numThreadsLeft == 0) {
                delete_nodes(claimed_list);
            } else if (claimed_list) {
                node* last = claimed_list;
                while (last->next != nullptr) {
                    last = next;
                }

                last->next = to_be_deleted;
                while (!to_be_deleted.compare_exchange_weak(last->next, claimed_list))
                    ;
            }
        } else {
            old_head->next = to_be_deleted;
            while (!to_be_deleted.compare_exchange_weak(last->next, claimed_list))
                ;
            threads_in_pop--;
        }
    }

    void delete_nodes(node* nodes) {
        while (nodes) {
            node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

   public:
    LockFreeStack() {
        head = nullptr;
    }

    void push(T const& data_) {
        const node* new_node = new node(data_);
        new_node->next = head.load();

        /**
         * If head hasn't changed by another thread
         * Compare exchange (haed == new_node->next) will be true, head will be replaced with new_node
         *
         * If head has been changed by another thread to say head2
         * Compare exchange (head == new_node->next) becomes false, new_node->next will be updated to head2
         *
         * Refer compare exchange section in section 6's readme.
         */
        while (!head.compare_exchange_weak(new_node->next, new_node))
            ;
    }

    std::shared_ptr<T> pop() {
        threads_in_pop++;

        node* old_head = head.load();

        // head.store(old_head->next);
        // result = old_head->data;
        // Thread safe version of above two looks as follows
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
            ;

        std::shared_ptr<T> res;
        if (old_head) {
            res.swap(old_head->data);
        }

        try_reclaim(old_head);

        // delete old_head (but this is not thread safe?)
        return res;
    }
};