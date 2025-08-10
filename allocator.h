#ifndef LINALG_ALLOCATOR_H
#define LINALG_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALLOCATOR_STORAGE_STACK_SIZE (300U)

typedef struct {
    uint8_t buffer[ALLOCATOR_STORAGE_STACK_SIZE];
} allocator_storage_stack_t;

typedef struct {
    void* buffer;
    size_t buffer_size;
} allocator_storage_heap_t;

typedef union {
    allocator_storage_stack_t stack;
    allocator_storage_heap_t heap;
} allocator_storage_t;

typedef enum {
    ALLOCATOR_TYPE_HEAP,
    ALLOCATOR_TYPE_STACK,
    ALLOCATOR_TYPE_NONE,
} allocator_type_t;

typedef struct {
    allocator_type_t type;
    allocator_storage_t storage;
} allocator_t;

void* allocator_new(allocator_t* allocator, size_t size);
void allocator_delete(allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif // LINALG_ALLOCATOR_H