-- Memory Allocator Project --

I created a memory allocator that resembles the memory allocator C uses.
  - AKA: Recreated the 'malloc' and 'free' function in C.

Fully dynamic memory allocator that efficiently reuses, recycles, and returns memory and memory blocks.
  - Uses coalescing and a first fit policy in order to restructure the blocks after free, minimizing waste.

Ran and tested with gcc and driver files, which are the following:
  driver_1570x1600.c, driver_alloc_1.c, driver_alloc_1_24.c, driver_alloc_8.c, driver_alloc_8_free.c, driver_free.c
  
 - Which all test different allocation types, including edge cases. *This allocator works with a max of ~1500 allocations at a time, depending on size.
