## Section 1: Thread Management

### Miscellaneous

- Parallel processing on a single core system will have no benefits as the processor runs only one process at a time and will context switch between multiple processes.
- Parallel processing will have performance gain on a multi core system.
- Different processes run in separate memory spaces while different threads (of the same process) run in a shared memory space.
- **Task Level Parallelism:** Multiple threads / processes perfromning different tasks on different or same type of data.
- **Data Level Parallelism:** Multiple threads performing same task on different sets of data.

### Parallelism vs Concurrency

| Concurrency | True Parallelism |
|:------------|:-----------|
| Mutliple processes running on a single or small number of cores using context switch. Illusion of simultaneous execution by running processes sequentially with context switching | True parallelism occurs when there are enough cores to run each process on different cores. Then each process will indeed run simultaneously |

### Joinability

- Properly constructed thread object represents an active thread of execution in hardware level. Such thread is **joinable**.
    - A thread constructed using default constructor i.e no callable provided is not a proper thread i.e its not running and hence is not callable.
- For every joinable thread, we must call either **`join`** or **`detach`** function post which the thread is no longer joinable. 
- If we forget to **`join`** or **`detach`** then at the time of destructor call to thread object **`std::terminate`** is called.
- Any program with **`std::terminate`** call is referred unsafe.

### Join and Detach

- **`Join`** introduces a single synchronized point between launched thread and thread it launched from.
    - Execution of the thread that calls join is blocked until the launced thread finishes.
- **`Detach`** sepearates the launced thread from the thread object which it launched from, allowing execution to continue independently.
    - Any allocated resources will be freed once the thread exits.

### Join In Exception Scenarios

- In real world we ideally don't call join immediately as it will block the caller immediately.
- Other operations before join if lead to runtime error will prevent the join from getting executed.

#### RAII

- **Resource Acquisitin is Initialization**
    - Constructor acquire resources, destructor releases resources.

### Thread with Parameters

- To pass reference you have to explicitly use **`std::ref`**
- Be careful when you are detaching threads after you pass reference to that thread as detached thread can outlive the lifetime of the referenced object in the thread it detached from.
    - Resulting in a segfault.
    - Pass by value in such cases where you have to detach.

### Transfer the ownership of thread

- Thread objects are not copy constructor or copy assignable.
- Use move constructor or **`std::move`**.
- Owner of the object is responsible for managing the object's lifecycle.

### Some miscellaneous functions

- **`thread_obj.get_id()`**: Get the id of the active thread. For improperly constructed thred (default constructed thread) or a non-joinable thread will retun an id of 0.
- **`sleep_for`**: Blocks the execution for at least the specified duration. May be a bit longer because of scheduling and context switch delays.
- **`std::this_thread::yield`**: Make the processor give up the current time slice and re-insert the thread into the scheduling queue. The amount of time that expires until the thread is executed again is usually entirely dependent upon the scheduler.
- **`std::thread::hardware_concurrencty()`**: Returns the number of concurrent threads supported by the hardware implementation. The value should be considered only a hint.
    - Usually equal to number of logical cores.

### Thread Local Object

- When you declare a variable **`thread_local`**, then each thread is going to have its **own distinct object**.
- The storage duration is the entire execution of the thread in which it was created, and the value stored in the object is initialized when the thread is started.

## Section 2

### Locking Mechanisms

- We need locking mechanism when we share memory across threads.

### Invariants

- Statements that are always true for a data structure.
    - For list data structure, size variable contains number of elements in the list.

- Invariance may break in multi threaded environment.
    - Two threads trying to delete same node in a linked list

- Race condition:
    - Outcome depends on relative order of execution of operations on two or more threads.
    - If race condition results in a broken data structure (breaks invariance) then theses are problematic race conditions.

### mutexes

- Synchronization primitive to provide mutually exclusive access of shared data for multiple threads.
- E.g. multiple threads trying to insert into std::list. Most if not all DS in STl are not thread safe data structure.
    - Pushing a element to list is not a atomic operation.
- Explicit Locking-Unlocking is burden. C++ provides **`lock_guards`**.

#### `Lock_Guards`

- Mutex wrapper that provides a convenient RAII-style mechanism for owning a mutex for the duration of a scoped block.
- When **`lock_guard`** object is created, it attempts to take ownership of the mutex it is given.
- When control leaves the scope in which the **`lock_guard`** object was created, the **`lock_guard`** will get destructed and mutex will be released.

### Pit Falls related to mutex

- Returning pointer to lock protected data.
    - Though you have protected the data with a lock but the caller once got the pointer can do anything.
- Passing lock protected data to some function you don't have control of. 
    - You don't know what the function is doing, it might store a reference to the protected member and access it in unsafe manner.

### Race conditions inherit from interface

- Race between empty() and top();
- Assume both empty and top take lock before proceeding.

```cpp
if(!stk.empty()) {
    int value = stk.top();
    std::cout<<"Value "<<value<<endl;
    stk.pop();
}
```

- Thread 1 and Thread 2 can both reach inside the if block. Thread 1 can pop and when thread 2 tries to access the top it will break.
- Race conditions happened even when individual operations prevent access via mutex. 

#### Solution

- Change the interface to combine top and pop function into a single function **`pop_top`**.
- This will internally does an empty check and does top and pop all locked by mutex.
- See `thread-safe-stack.cpp`.

### DeadLocks

- See `deadlock.cpp`

### **`unique_locks`**

- General Purpose mutex ownership wrapper used to manage mutexes.
- Unlike **`lock_guard`** it does not have to acquire the lock from the associated mutex.
- `unique_locks` are neither copy constructibe nor copy assignable.
- But they are move constructibel and move assignable.
- See `unique-locks.cpp` to see how we solve the problem in `deadlock.cpp`