## Concerns So Far

- Invariants were upheld at all times.
- Avoiding race conditoins inherit from interface.
- Handling exception scenarios.
- Avoiding deadlocks at all costs.

In Section 3 we created a thread safe_queue:
- To make sure thread safety (invariants are upheld at all times)
    - We had a single mutex.
    - Each operation must acuire lock first.
    - To remove race condition inherit from interface. We combined front and pop to a single pop();
- To avoid the exception scenario:
    - We stored the shared pointers in the queue instead of direct values
- No deadlocks
    - Because there's only one mutex.

But, there is no concurrency here because regardless of how many threads are concurrenlty accessing this data structure only a single thread will be allowed to proceed because we have a single mutex.
    - Let's implement queue from scratch allow maximum concurrency.


## GOAL: Allow maximum concurrency possible in the data structure.

-> See [01-thread-safe-queue-maximal-concurrency.cpp](01-thread-safe-queue-maximal-concurrency.cpp)

## Parallel STL

- Extension of STL
- With C++17 we can specify algorithms about user prefered way of execution, parallel or sequentail.
- Not all algorihtms have this facility and compiler will decide which way to run the algorithm based on the existing resource consumption.
- We have the ability to tell the compiler that if possible please do it in parallel. using execution policy. For e.g.
  ```cpp
  std::sort(std::execution::par, vec.begin(), vec.end());
  ```
- Execution Policy affects the algorithm in multiple ways:
  - Algorithm's complexity.
  - Behaviour when exception is thrown.
  - Where, how and when steps of algorithms are executed.

Actual performance gain happens when we have enough hardware to overcome the overheads due to extra algorithm complexity and operating system level thread management tasks.

## Factors affecting the performance of concurrent code

### Division of Data

- Take into consideration how much data each thread has to load in memory.
- For example for matrix multiplation instead of calculating row of output matrix per thread, perhaps we could do the division in 2D as in each thread calulates 2x2 part of output matrix.


### How many processores

- **We usually launch, number of threads < hardware_concurrency.** But what if multiple localtions in code are doing this? Leads to over subscription.
- **Use std::async: It has application levle visibility of number of threads**, so let compiler decide when to launch a thread or defer execution or not. But what if there are multiple applications running?
  - We need global visibility of number of threads, might have to make os calls. 

### Data contenction and cache ping pong

- L1 Cache for each thread might update and if they are modifying same data then cache continuosly gets inalidated.
- Ensure data isolation?

### False sharing

- Multiple threads share the same cache line. Leads to continuous evictions of data.
- How do we control this?
  - Data locality
