## Section 2: Mutexes and Locks

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
- Problem with return by value:
    - Returning by value requires memory allocation and can cause problems if there are memory constraints. Two solutions:
        - Store the shared ptr in stack and return the shared ptr.
        - Have the caller allocate the memory and pass reference to it so we can update it.

### DeadLocks

- See `deadlock.cpp`
- Lock while joining without mutexes: t1 calls `t2.join()` and t2 calls `t1.join()`

### **`unique_locks`**

- General Purpose mutex ownership wrapper used to manage mutexes.
- Unlike **`lock_guard`** it does not have to acquire the lock from the associated mutex.
- `unique_locks` are neither copy constructibe nor copy assignable.
- But they are move constructable and move assignable.
- See `unique-locks.cpp` to see how we solve the problem in `deadlock.cpp`