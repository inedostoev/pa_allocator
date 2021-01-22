# A block allocator for embedded systems

This is C/C++ library of a block memory allocator. 

This allocator able to allocate memory one block of a fixed size from a static pool. Block and pool sizes are fixed during program execution, but can be customizable during project build. The module is adapted adapted to work on Embedded platforms of various bit sizes in a multitasking environment (RTOS).

# Usage
This library is also designed to be testable in standalone mode using `ceedling`. To run the test suite, [install](http://www.throwtheswitch.org/ceedling) `ceedling` and then run:
```
ceedling clean
ceedling test:all
```
The following variables must be set to something useful for the library to work at all:

* `PA_HEAP_ADDR` - must be set to the starting address of the heap.

* `PA_HEAP_SIZE` - must be set to the size of the heap.

* `PA_BLOCK_SIZE` - must be set to the block size of allocator. The block size must be greater than or equal to the platform pointer size

Call `pa_init()` to initialize the allocator.

In `src/cfg.h` you can configure macros to implement the critical section in different applications.

# API

The following functions are available for your application:

* `void *pa_malloc(void);`
* `void *pa_calloc(void);`
* `void  pa_free(void *ptr);`

# TODO
* More detailed tracing function (for tracing `pa_heap`, `pa_root` and linked list);
* Adding advanced methods for detecting pointers that have not been allocated but are trying to be freed. This can lead to memory leaks due to the list loop. Now all this work lies with the developer.

