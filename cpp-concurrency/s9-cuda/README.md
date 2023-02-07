## Basic Steps of a CUDA program

- Initialization of data from CPU.
- Transfer data from CPU context to GPU context.
- Kernel launch with needed grid/block size.
- Transfer results back to CPU context from GPU context.
- Reclaim the memory from both CPU and GPU.

## Elements of CUDA program

- Host Code (Sequential): Runs on CPU
- Device Code (Parallel): Runs on GPU
- Cuda Kernel:
    ```c++
    // Return type has to be void
    __global__ void hello_cuda() {
        printf("Hello CUDA word");
    }
    ```
- Launching the cuda kernel
    ```c++
    // 20 threads will run ?
    hello_cuda<<< 1, 20 >>>(); // This is an async function call

    cudaDeviceSynchronize(); // Forces the host code to wait here untill all kernels are done completing
    cudaDeviceReset(); // Reset the device
    ```

### Grid and Block

- Grid: Grid is a collection of all the threads launched for a kernel.
- Block: Threads in a grid is organized in to groups called group blocks.
```c++
dim3 block(8, 2, 1); // Maximum number of threads per block is 1024 so x * y *z <= 1024
dim3 grid(2, 2, 1); // A grid can have: x <= 2^32 -1 , y <= 65536 and z <= 65546

// We have a 2 x 2 grid, each element of grid is a block of 8 x 2 = 16 threads

hello_cuda<<grid, block>>
```

## Organization of threads in a CUDA program

### Variables identifying a thread

- `threadIdx`: Each thread will have a unique dim3 variable identifying its position within the thread block.
- `blockIdx`: Each thread will have a unique dim3 variable identifying it's block in the grid.
  - Same across all threads in a block.
- `blockDim`: A dim3 variable that gives you the dimension of the block of which the thread is part of.
  - Same acrooss all threads in a block
- `gridDim`: A dim3 variable that gives you the dimension of the grid.
  - Same across all threads.

### Unique Global Index for a thread

```c++
threadOffset = (blockIdx.x * blockDim.x, blockIdx.y * blockDim.y, blockIdx.z * blockDim.z);
threadGlobalIdx = threadIdx + threadOffset;
```

- The memory access pattern depends on the way we calculate our global index.
- **We usually prefer to calculate global indices in a way that, threads with in same thread block access consecutive memory lcoations or consecutive elements in the array.**

## Performance of CUDA application

### Factors

- Execution Time.
- Power Consumption.
- Floor Space.
- Cost of Hardware.

### Trial and Error

- Running the cuda program with different grid, block, shared memory, cache, memory access configurations and choose the best configuration based on the execution time.

## CUDA memory transfer

- `cudaMemCpy(destination_ptr, source_ptr, size_in_byte, direction)`
  - Direction
    - `cudamemcpyhtod`: Host to device
    - `cudamemcpydtoh`: Decice to host
    - `cudamemcpydtod`: Device to device

- Memory Allocation
  | CPU      | GPU          |
  |----------|--------------|
  | `malloc` | `cudaMalloc` |
  | `memset` | `cudaMemset` |
  | `free`   | `cudaFree`   |

## Error Handling in CUDA program

### Runtime errors

- Usually well handle it using exceptions.
- **All cuda function other than kernel launch** returns `cudaError` enum.
- `cudaGetErrorString(error)`
- e.g
    ```c++
    int *d_a;
    eorror = cudaMalloc((int **)&d_a, NUM_BYTES);
    if (error != cudaSuccess) {
        fprintf(stderr, "Error : %s\n", cudaGetErrorString(error));
    }
    ```

## Cuda device properties

- Depending on CUDA device compute compatibility, properties of CUDA device is going to vary.
- When we program a CUDA application to run on device with multiple compute acpabiliteies, then we need a way to query the device properties on the fly.