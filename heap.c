#include "heap.h"

char heap[HEAP_SIZE];

typedef struct FreeHeader
{
    unsigned int size;
    struct FreeHeader * next;
    struct FreeHeader * prev;
} FreeHeader;

typedef unsigned int AllocHeader;


FreeHeader * FreeList = (void *) heap;


void InitHeap()
{
    printf("Start of heap: %u\n", FreeList);
    printf("End of heap: %u\n\n", (void *) FreeList + HEAP_SIZE);
    FreeList -> size = HEAP_SIZE;
    FreeList -> next = NULL;
    FreeList -> prev = NULL;
}


void printFreeList()
{
    FreeHeader * ptr = FreeList;

    printf("\nFree List:\n");

    if (ptr != NULL)
    {
        while (ptr -> next != NULL && ptr -> next != (void *) 0xffffffff00000000)
        {
            printf("(%u <heap+%u>, ", ptr, (void *)ptr - (void *)heap);
            printf("%u) -> ", ptr -> size);

            ptr = ptr -> next;
        }

        printf("(%u <heap+%u>, ", ptr, (void *)ptr - (void *)heap);
        printf("%u)\n\n", ptr -> size);
    }
}


void * alloc(size_t reqSize)
{
    static int init = 1;
    if (init) InitHeap();
    init = 0;

    FreeHeader * cur = FreeList;
    AllocHeader * address = NULL;

    while (cur != NULL && cur -> size < reqSize + sizeof(AllocHeader) )
    {
        cur = cur -> next;
    }

    if (cur != NULL)
    {
        if (cur -> size > reqSize + sizeof(AllocHeader) + sizeof(FreeHeader))
        {
            address = (void *) cur + (cur -> size - reqSize - sizeof(AllocHeader));
            *address = reqSize + sizeof(AllocHeader);
            address++;
            cur -> size -= reqSize + sizeof(AllocHeader);
        }
        else
        {
            if (cur -> prev == NULL)
            {
                FreeList = cur -> next;
            }
            else
            {
                cur -> prev -> next = cur -> next;
            }

            address = (void *) cur;
            *address = cur -> size;
            address++;
        }
    }

    if (address != NULL) printf("Allocated: (%u <heap+%u>, %u)\n", address, (void *)address-(void *)heap, *(address - 1));
    else printf("Allocation failed\n");

    printFreeList();
    
    return (void *) address;
}


void free(void * address)
{
    printf("Freeing: %u <heap+%u>\n", address, address-(void *)heap);

    FreeHeader * ptr, * block;
    AllocHeader * sizeptr;

    if (address > (void *) heap && address < (void *) heap + HEAP_SIZE)
    {
        ptr = FreeList;
        sizeptr = address - sizeof(AllocHeader);
        block = (void *) sizeptr;
        block -> size = *sizeptr;

        if (ptr == NULL)
        {
            FreeList = block;
            block -> next = block -> prev = NULL;
        }
        else if (block < ptr)
        {
            FreeList = block;
            block -> next = ptr;
            block -> prev = NULL;

            if ((void *) block + block -> size == (void *) ptr)
            {
                block -> next = ptr -> next;
                block -> size += ptr -> size;
                if (block -> next != NULL) block -> next -> prev = block;
            }
        }
        else
        {
            while (ptr -> next != NULL && block > ptr -> next)
            {
                ptr = ptr -> next;
            }

            if ((void *) ptr + ptr -> size + block -> size == (void *) ptr -> next)
            {
                ptr -> size += block -> size + ptr -> next -> size;
                ptr -> next = ptr -> next -> next;
                if (ptr -> next != NULL) ptr -> next -> prev = ptr;
            }
            else if ((void *) ptr + ptr -> size == (void *) block)
            {
                ptr -> size += block -> size;
            }
            else if ((void *) block + block -> size == (void *) ptr -> next)
            {
                block -> size += ptr -> next -> size;
                block -> next = ptr -> next -> next;
                ptr -> next = block;
                block -> prev = ptr;
                if (block -> next != NULL) block -> next -> prev = block;
            }
            else
            {
                block -> next = ptr -> next;
                block -> prev = ptr;
                ptr -> next = block;
                if (block -> next != NULL) block -> next -> prev = block;
            }
        }
    }
    printFreeList();
}