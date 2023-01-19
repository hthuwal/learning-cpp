## Jthreads

Why another form of threads?

- Current threads has to explicity either join or death otherwise on destruction std::terminate exception is raised. **What if the threads itself manage its lifecycles?**
    - **Jthreads manages its lifecycle implicitly.**
        - When jthreads destructor is called, 
        - interrupt the work its doing.
        - the thread will call join function on itself if thread itself.
        - Detach still needs to be done explicitly.
- We cannot interrupt standard threads execution after we launch it, easily without sharing flags or variables? **What if it's built into specification?**
    - Jthread is working with `stop_token` to achieve interruptible.
    - We can introduce condition checks with stop_token to specify interrupt points. Check `01-jthreads.cpp`

### Traditioanl **Subroutines**: Normal Functions

- Normal Function Call
- Subroutine's stack frame is on top of caller's stack
- Caller's execution is blocked untill subroutine's finishes
- Two Stages:
    - Invoke: Initialization of subroutine.
    - Finalize: Finish of subroutine and deallocations.

## Coroutines

- `-fcoroutines`
- Four Stages
    - Invoke
    - Suspend
    - Resume
    - Finalize
- A function that contain `co_await` or `co_yield` or `co_return` is treated as a coroutine by compiler
- Coroutines state object is created in heap and is brought in and out of execution stack on wait and resume respectively.
- Coroutine is stack less. It does not create any stack for iteself.
    - It will copy its stack frame to running stack for execution on resume.
    - This is a performance boost as no context switching is required.

### Coroutine consists of three parts

- A promise object (different from promise)
    - User defined `promise_type` object. Name should be exactly this
    - Manipuate inside coroutines
    - Return result via this object
- Handle
    - Non ownning handle whichu use to resume or destroy coroutine from outside.
- Coroutine state
    - Heap allocated
    - Containse promise object, arguments to coroutine, representation of suspension points and local variables.

### C++ Barriers

- Barrier is a synchronization mechanism that make threads to wait until the required number of threads has reached a certain point in code. Once all the threads have reached the barrier, they're all unblocked and may proceed.
- Barrier get's reset once opened and can be reused.