## Terminologies

### Blocking vs Non Blocking 

- Algorithms and data structures that use mutexes, condition variable, and futures to synchronize the data are called blocking.
- DS and algorithms that don't use blocking library functions are said to be non-blocking.

### Lock free vs Wait free

- With Lock free data structures some thread make progress with every step.
- With wait free data strucutres every thread can make progress regardless of what other threads are doing.

## Stack Pop thread safe delete

### Problem

Our problem is the lack of guarantee that only one thread accessing the node to be deleted.

### Solution: Memory reclaim mechanism using thread counting

- Count number of threads in the pop function.
- If the thread count in the pop function is more than 1, then we cannot delete the node saftely.
  - In such cases we are going to add that node to list called to be deleted.
  - Not ideal for heavy load where they may not be only one thread popping. Out to be deleted list will continue to grow.
- If the thread count become 1, then all the nodes which are safe for delete will be deleted by that particular thread.

### Solution: Memory reclaim mechanism using hazard pointer

Too complicated

### Solution: Reference Counting

#### Read C++ Concurrency in action to understand this

There are multiple ways to do reference counting. Here we are going to use a methodology which uses two count for each node: 
- External Count: Is kept alongside pointer to node
  - Increased every time pointer is read.
- Internal Count for each node.
  - When reader is finished with the node, it decreases the internal count.
- So a read operation increments external count by 1 and decreases internal count by 1.
- The sum of these values is the total number of active references to a particular node.