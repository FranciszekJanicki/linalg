#include "allocator.h"
#include <assert.h>
#include <stdlib.h>

void* allocator_new(allocator_t* allocator, size_t size)
{
    assert(allocator != NULL);

    allocator_delete(allocator);

    if (size <= sizeof(allocator->storage.stack.buffer)) {
        allocator->type = ALLOCATOR_TYPE_STACK;

        return allocator->storage.stack.buffer;
    }

    allocator->storage.heap.buffer = malloc(size);
    if (allocator->storage.heap.buffer != NULL) {
        allocator->type = ALLOCATOR_TYPE_HEAP;
        allocator->storage.heap.buffer_size = size;
    } else {
        allocator->type = ALLOCATOR_TYPE_NONE;
        allocator->storage.heap.buffer_size = 0UL;
    }

    return allocator->storage.heap.buffer;
}

void allocator_delete(allocator_t* allocator)
{
    assert(allocator != NULL);

    if (allocator->type == ALLOCATOR_TYPE_HEAP &&
        allocator->storage.heap.buffer != NULL) {
        free(allocator->storage.heap.buffer);

        allocator->storage.heap.buffer = NULL;
        allocator->storage.heap.buffer_size = 0UL;
    }

    allocator->type = ALLOCATOR_TYPE_NONE;
}
