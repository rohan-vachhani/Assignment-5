#include <stdio.h>

#define HEAP_SIZE 1024

void InitHeap();
void printFreeList();
void * alloc(size_t reqSize);
void free(void * address);