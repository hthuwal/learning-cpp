## Section 3: Thread Communication, Condition Variables and Futures


### Condition Variable

- Condition variable is basic mechanism for waiting for an event to be triggered by another thread.
- Condition variable si associated with some **event**, and** one or more threads can wait for that event to happen**. If **some thread has determined that the event is satisfied it can then notify one or more of the threads waiting for that condition variable**, and wake them up and allow them to continue procesing.
- Example: Overnight bus example
    - **Event**: Arriving at the destination.
    - **Waiting Threads**: Sleeping Passenger.
    - The thread that determine event to happen and notiy it to waiting threads: The driver
- To wait on a condition variable we need to provide two thing:
    - A unique lock
    - Condition to check after wake up as lambda/function.
- Condition variable should be able to call lock/unlock on the associated mutex. This is why we use unique locks. We cannot use lock_guards because it doesn't provide lock/unlock functionality.
- Condition variable wake up can be due to:
    - Notification from another thread.
    - It can be spurious wake up from operating system.
        - It will be fallback to sleep because the expression would still be false.
        - Is that why an expression is needed? This seems like something that could have been solved (Explore!). 😕
- cv.notify_one() --> Only one of threads waiting on the condition variable will wake up randomly.
- cv.notify_all() --> All the waiting threads can wake up.

### Async Tasks

- A synchronous operation blocks a process till the operation completes.
- An asynchronoous operation is non-blocking and only initiates the operation. 
- The caller could discover completion by some other mechanism (Futures?)
  ```
  Operation 1              Operation 1
       │                        │
       │                        │
       │                        │
       │                        │
       │                        │
       │                        │
       ▼                        │
       └─────►Operation 2       ├─────►Operation 2
                   │            │           │
                   │            │           │
                   │            │           │
                   │            │           │
                   │            │           │
                   │            │           │
       ◄───────────▼            │◄──────────▼
       │                        │
       │                        │
       │                        │
       │                        │
       │                        │
       ▼                        ▼
  ```
- How to get result out of async operations.
    - First, creator of the asynchronous task have to obtain the future associate with asynchronous task.
    - When creator of async task need the result of that async task it called get method on future.
    - get method may block if the asynchronous opoeration has not yet completed its execution.
    - When the asynchronous operation is ready to send a result to the creator, it can do so by modifying shared state that is linked to the creator's `std::future`.

  ```
                      ┌───────┐
                      │Creator│    ┌────────────────┐
                      └───┬───┘    │ Async Operation│
     ┌───────────────┐    │        └────────────────┘
     │ Get the future│    │
     │associated with│    ├─────────────────►┐
     │ the async task│    │                  │
     └───────────────┘    │                  │
                          │                  │
                          │                  │
                          │                  │
  ┌─────────────────┐     │                  │
  │ If the result of│     │                  │
  │async is needed. │     ▼                  │
  │  Wait for it.   ├─────►                  │
  └─────────────────┘                        │
                                             │
                                             │  ┌──────────────────────────────┐
                                             │  │Once the async operation ended│
                                             ▼  │it can update the shared state│
                          ◄──────────────────┘  │in future so that the creator │
                          │                     │thread can access the results.│
                          │                     └──────────────────────────────┘
                          │
                          │
                          ▼
  ```
- `std::async(std::launch_policy, Function &&f, Args&&... args);`
    - launch_policy
        - `std::launch::async` -> function will Run on a different thread
        - `std::launch::deferred` -> function will run on the creator thread when `future.get` is called.
        - `std::launch::async | std::launch::deferred` -> compiler will decide what to do. This is how we avoid over subscription, compiler decides this based on available cpu resources.
- Template parameter of the future must match the return type of the function being called in async.


### Package Tasks

- `std::package_task` more generic version of async task?
- Wraps any callable target so that it can be invoked asynchronously.
- It's return value or exception thrown, is stored in a shared state which can be accessed through `std::future` objects.
- `std::package_task<return_type(arg1_type, arg2_type)> task(callable_object)`
- Package task does not run automatically after construction. It has to be run explicitly. Like: task(arg1, arg2);

### Promises

- Transferring the values between threads.
- Technically even `package_task` can be implemented using `std::promises`
- Each `std::promise` object is parired with a `std::future` object.
- A thread with access to the `std::future` object can wait for the result to be set, while another thread that has access to the corresponding `std::promise` object can call `set_value()` to store the value and make the future ready.
- Future has to wait for promise to set the value.
- Avoid deadlock:
    - If the thread supposed to set value (T1) calls join on the future thread (T2) before setting the promise value.
    - Future thread calls future.get().
    - In this case the T1 is waiting for T2 to finish before it can set value and T2 is waiting for T1 to set value before it can finish.

### Promises + Exception

- Promise has `set_exception()` to convey an exception from promise to future.

### Shared Futures, Waiting from multiple threads

```cpp
terminate called after throwing an instance of 'std::future_error'
  what():  std::future_error: No associated state
```
- future.get() is oneof. 
- Once `future.get()` is called, afterwards `future` object becomes invalid.
- We can use `future.valid()` to check if future is valid or not but beware of race conditions between threads.
- **Solution `std::shard_future`**

**So, if multiple threads are going to call future.get() we must use a shared_future.**