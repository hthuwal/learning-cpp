# Atomicity

## Atomic Operation

- An indivisible operation. We cannot observe such an operation half-done from any thread in the system.
- i++ is not an atomic operation if i is an int. It's a read-modify-write operation.
    - Read value of i
    - Increment that value by 1
    - Store the modified value


## Atomic Type

- If all the operation on particular type is atomic then we refer such types as atomic types.
- E.g `std::atomic<char>` aka `std::atomic_char`.
- All atomic types are:
    - Neither copy assignable not copy constructible.
    - Can assign non atomic types to atomic types.
    - Can construct atomic types using non atomic types.

### Functions on atomic type except atomic flag

- [02-atomic-bool.cpp](02-atomic-bool.cpp)
    - `Is_lock_free`
    - `store`
    - `load`
    - `exchange`: Replace the stored value with new one and atomically retrieve the original one.
- [03-compare-exchange.cpp](03-compare-exchange.cpp)
    - `compare_exchange_weak`
    - `compare_exchange_strong`

### Atomic Flag

- Basic and intended as basic building blocks only.
    - Atomic flag should initialized with `ATOMIC_FLAG_INIT`.
    - Only two functions `clear()` and `test_and_set()`
    - Always lock free.
- The atomicity of these types may be achieved by locks in some operating systems. But in major OS's these types are implemented without locks. To check this use
    - `is_lock_free`

### Compare exchange operations

- [Code](03-compare-exchange.cpp)
- One of the corner stone operations in atomic realm.
- Compare Exchange:
    - `X.compare_exchange_weak/strong(T& expected, T desired)`
    - **Compare the value** of the atomic variable `X` with supplied (reference) expected value. 
    - **If they match stores the supplied desired value**. If the store succeeds return true.
    - **If they dont match the expected value is updated** with the actual value. return false.
- Compare Exchange Weak vs Strong:
    - Weak does not gaurantee the successfulness of the operation even if the expected value matches the value of atomic variable.
        - Happens mostly on machine that does not have single compare and exchange instruction.

    - Strong gauranteed to be atomic and returns  success if the expected value matches the value of atomic varaible.
        - For this gaurantee it may use more instructions on machines where comapre exchange instruction is not present.

## Atomic Pointers

- `Atomice<T*>` is a atomic form of pointer.
    - Does not mean object pointed to is atomic.
    - The pointer itself is atomic.
- Extra operations:
    - `fetch_add`, `+=` 
    - `fetch_sub`, `-=`
    - `++`
    - `--`

## Atomic Class Template User defined types

- Type must have trivial copy-assignment operator
  - Must not have virtual functions.
  - Every non static members should have trivial copy-assignment operator (not virtual).
  - Not a descendent of virtual base class.
    - Base class must use a compiler generated copy assignment operator. 

- **All this essentially ensures that type must be bitwise equality comparable.**

## Atomic Pointer and Thread Relationship

- [Code](05-thread-atomic-relationships.cpp)

# Memory Ordering

[Code](06-oredering-options-intro.cpp)

- `memory_order_seq_cst`: Sequential Consistent, Default if not specified
- `memory_order_relaxed`
- `memory_order_acquire`
- `memory_order_release`
- `memory_order_acq_rel`: Aquire Relaxed
- `memory_order_consume`

### `memory_order_seq_cst`

- [Code](07-mem-order-seq.cpp)
- Default and easiest memory ordering option.
- This implies that the behaviour of the program is consistent with the simple sequential view of the world.

### Instruction reordering

- Store Buffers in CPU:
    - Writing to memory takes longer than read. So writes are buffered.
    - So, there may be scenarios, even though the write happens before read in code, write may execute after read because of the store buffers.

- Many things can happen in between source code and actual code, which may cause reordering:
  - Compiler:
    - Sub expression elimintation.
    - Register Allocation.
  - Cache:
    - Store Buffers
    - Private shared cache
  - Processor:
    - Prefetch
    - Speculation
    - Overlap writes

- Re-ordering Example
  - First statement is uselesess so is eliminated.
    ```  
    ┌─────────────────┐
    │ X = 5;          │                ┌─────────────────┐
    │                 │                │ Y = "GO GO GO"; │
    │ Y = "GO GO GO"; ├───────────────►│                 │
    │                 │                │ X = 9;          │
    │ X = 9;          │                └─────────────────┘
    └─────────────────┘
    ```
  - Depending on the memory layout, compiler may exchange inner and outer loops whichever leads to linear memory access
    ```c++
    ┌────────────────────────┐          ┌────────────────────────┐
    │for(i=0; i<rows; i++)   │          │for(j=0; j<cols; j++)   │
    │  for(j=0; j<cols; j++) ├─────────►│  for(i=0; i<rows; i++) │
    │     a[j*rows + i] += 9;│          │     a[j*rows + i] += 9;│
    └────────────────────────┘          └────────────────────────┘
    ```

- In a multithreaded environment, if multiple threads access shared variables we have to explicilty mention to the compiler about mutually exclusive access.
  - We achieved this earlier using mutexes. Which by maintaining exclusivity also ensured sequential memory ordering.

### `memory_order_relaxed`

- [code](08-mem-order-relaxed.cpp)
- Completely opposit to the `memory_order_seq_cst`
- View of the threads does not need to be consistent to each other.
- No restriction on instruction re-ordering.
- Out come will depend on whether you r processor is strongly ordered or not. (ARM processors are known for weakly ordering?)
- Why use this order at all?
  - `memory_order_seq_cst` imposes sequential constraints, which may force compiler to do flushes, may not do optimizations and impose other overheads.
  - If not required we can improve performance by telling the compiler that sequential constraints are not needed.


### `memory_order_acuire` and `memory_order_release`

- They create synchronization pair:
  - All writes till `store` using `release` by one thread are visible to the thread which does its `read` using `acquire` first.
- [Code](09-mem-order-acq-rel.cpp)

#### Categorization of atomic operations and memory orders they can use

- Load / acquire operations: `load`
  - relaxed, acquire, consume, seq_cst
- Store / release operations: `store`
  - relaxed, release, seq_cst
- Read modify write / acquire release operations: `Exchange`, `compare_exchange_weak`, `compare_exchange_strong`, `fetch_sub`, `fetch_add etc.
  - All: relaxed, consume, acquire, release, acq_rel, seq_cst

### Transitive synchronization

- [Code](10-transitive-synchronization.cpp)
- We can synchronize two threads without any explicit release and acquire ordering mechanism.

### `memory_order_consume`

- Special case of `memory_order_acquire` which limits the synchronization of data to direct dependencies.
- Two New relationships
  1. Carries-a-dependency-to:
    - Applies within a thread.
    - If result of an operation A is used as operatnd for and operation B, then A carries-a-dependency-to B
  2. Dependency-ordered-before:
    - Applies between threads.
    - **Performs the synchronization but limits the synchronization of data to the direct dependencies.**
- [Code](11-mem-order-consume.cpp)

### Release Sequences

- Chains of operation on same atomic variable. Release sequence proivde us a set of rules to logically deduct the synchronization points and outcomes.
- Release sequence Headed by A:
  - After a release operation A is performed on an atomic object M, the longest continuous subsequence of the modification on M that consists of:
    1. Writes performed by the same thread that performed A.
    2. Atomic read modify write operations made to M by any thread.

### Spin Lock Mutex Implementation

- [Code](12-spin-lock-mutex.cpp)