#include "allocator.h"
#include "allocator_debug.h"
#include "port.h"
#include <stddef.h>
#include <stdint.h>

typedef struct PageCB_t {
    struct PageCB_t* next;
} PageCB;

static uint8_t pool[ALLOCATOR_POOL_SIZE * ALLOCATOR_PAGE_SIZE];
static PageCB freePages[ALLOCATOR_POOL_SIZE];
static PageCB* firstFreePage;

void AllocatorInit(void)
{
    allocatorPortInit();
    allocatorPortEnterCriticalSection();
    for (int i = 0; i < ALLOCATOR_POOL_SIZE - 1; i++) {
        freePages[i].next = &freePages[i + 1];
    }
    freePages[ALLOCATOR_POOL_SIZE - 1].next = NULL;
    firstFreePage = &freePages[0];
    allocatorPortExitCriticalSection();
}

void* AllocatorAlloc(int* result)
{
    void* address = NULL;
    allocatorPortEnterCriticalSection();
    do {
        if (firstFreePage == NULL) {
            if (result != NULL)
                *result = ALLOCATOR_ERROR_NO_FREE_PAGES;
            break;
        }
        ptrdiff_t diff = firstFreePage - freePages;
        if (diff < (ptrdiff_t)0) {
            if (result != NULL)
                *result = ALLOCATOR_ERROR_INTERNAL;
            break;
        }
        size_t pageIndex = (size_t)diff / sizeof(PageCB);
        address = pool + pageIndex * ALLOCATOR_PAGE_SIZE;
        firstFreePage = firstFreePage->next;
        if (result != NULL)
            *result = ALLOCATOR_NO_ERROR;
    } while (0);
    allocatorPortExitCriticalSection();
    return address;
}

int AllocatorDealloc(void* ptr)
{
    if (ptr == NULL) { // like the behavior of free() from stdlib
        return ALLOCATOR_NO_ERROR;
    }
    ptrdiff_t diff = (uint8_t*)ptr - pool;
    if (diff < (ptrdiff_t)0 || (size_t)diff > sizeof(pool)) {
        return ALLOCATOR_ERROR_BAD_ADDRESS;
    }
    if ((size_t)diff % (size_t)ALLOCATOR_PAGE_SIZE != (size_t)0) {
        return ALLOCATOR_ERROR_NOT_ALIGNED;
    }
    allocatorPortEnterCriticalSection();
    {
        size_t pageIndex = (size_t)diff / (size_t)ALLOCATOR_PAGE_SIZE;
        freePages[pageIndex].next = firstFreePage;
        firstFreePage = &freePages[pageIndex];
    }
    allocatorPortExitCriticalSection();
    return ALLOCATOR_NO_ERROR;
}

#ifdef ALLOCATOR_DEBUG
int AllocatorDebugCountFreePages(void)
{
    allocatorPortEnterCriticalSection();
    int counter = 0;
    PageCB* p = firstFreePage;
    while (p != NULL) {
        p = p->next;
        counter++;
    }
    allocatorPortExitCriticalSection();
    return counter;
}

void* AllocatorDebugGetPoolBaseAdress(void)
{
    return (void*)pool;
}
#endif // ifdef ALLOCATOR_DEBUG