#include "mem.h"
#include <stdio.h>

int main() {
    Initialize_Memory_Allocator(1600);
    Mem_Dump();
  
    for (int i = 1570; i < 1600; i++) {
        printf("\nAllocating %d\n",i);
        char *p = Mem_Alloc(i);
        Mem_Dump();

        printf("\nFreeing %d\n",i);
        Mem_Free(p);
        Mem_Dump();
    }
    
    Free_Memory_Allocator();
    return 0;
}
