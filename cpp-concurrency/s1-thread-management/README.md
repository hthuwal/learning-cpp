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

