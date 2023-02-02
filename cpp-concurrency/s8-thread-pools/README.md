## Thread Pools

On most systems, it is impractical to have separate thread for every task that can potentially be done with other tasks, but you would still like to take advantage of the available concurrency where possible. A thread pool allows you to accomplish this.

### [Simple Thread Pool](01-simple-thread-pool.cpp)

#### Goal

- Tasks that can be executed concurrently are submitted to a pool which puts them on the queue of pending work.
- Each tasks is then taken off of the queue and executed by one of the worker threads and which on finishing the work loops back to pick another task.

#### Problems

- Lacks the Ability to wait on tasks.
- Do not have capability to get responses from threads.


### [Simple Thread Pool With Waiting on Submitting Tasks](02-simple-thread-pool-with-waiting-on-submitted-tasks.cpp)

#### Goal

- Ability to get futures for submitted task.
- Wait on the future.

#### Problems

- All threads may end up waiting for the tasks that haven't been scheduled yet. (Think recursive merge sort)
- We have no way of forcing the waiting threads to execute another tasks untill the results of waiting task arrives.
  - We have no way of switching tasks when they are executing.

### [Simple thread pool with capability to switch task untill future is ready](03-thread-pool-with-waiting-other-tasks.cpp)

#### Goal

- Add a public method in pool to just pop out another task from the work queue.
- Each task must keep calling this method until the future its waiting on is ready.

#### Problems

- When the number of processes increase, their is increasing contention on the queue. In this type of scenario, cache ping pong can be substantial time sink.

### [Thread Pool With Local Work Queue](04-thread-pool-with-local-queue.cpp)

### Goal

- One way to avoid cache ping pong is to use a separate work queue per thread. Each thread then posts new items to its own queue and takes work from the global work queue only if ther is no work on its own local queue.

#### Problem

- May cause unequal distribution of wo 

### [Thread Pool With Work Stealing](05-thread-pool-with-work-stealing.cpp)

### Goal

- One thread can steal work from other thread.
- Thread's local work queue must be sharable across threads.
  - Requires thread safe access.
  - To decrease contention, Use dqueue
    - Owner thread pops tasks from front.
    - Thief thread pops tasks from back.