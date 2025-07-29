#ifndef LINALG_VECTOR_H
#define LINALG_VECTOR_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define VECTOR_INDEX(VECTOR, INDEX) ((VECTOR)->data[(INDEX)])

typedef float vector_elem_t;
typedef size_t vector_size_t;

typedef enum {
    VECTOR_ERR_OK = 0,
    VECTOR_ERR_FAIL,
    VECTOR_ERR_NULL,
    VECTOR_ERR_DIMENSION,
    VECTOR_ERR_ALLOC,
} vector_err_t;

typedef void* (*vector_allocate_t)(size_t);
typedef void (*vector_deallocate_t)(void*);
typedef int (*vector_vprint_t)(char const*, va_list);

typedef struct {
    vector_vprint_t vprint;
    vector_allocate_t allocate;
    vector_deallocate_t deallocate;
} vector_interface_t;

typedef struct {
    vector_elem_t* data;
    vector_size_t size;
    vector_interface_t interface;
} vector_t;

vector_err_t vector_initialize(vector_t* vector,
                               vector_interface_t const* interface);

vector_err_t vector_deinitialize(vector_t* vector);

vector_size_t vector_size(vector_t const* vector);

vector_err_t vector_create(vector_t* vector, vector_size_t size);

vector_err_t vector_create_with_elem(vector_t* vector,
                                     vector_size_t size,
                                     vector_elem_t elem);

vector_err_t vector_create_with_zeros(vector_t* vector, vector_size_t size);

vector_err_t vector_create_from_array(vector_t* vector,
                                      vector_size_t size,
                                      vector_elem_t (*array)[size]);

vector_err_t vector_delete(vector_t* vector);

vector_err_t vector_resize(vector_t* vector, vector_size_t size);

vector_err_t vector_resize_with_elem(vector_t* vector,
                                     vector_size_t size,
                                     vector_elem_t elem);

vector_err_t vector_resize_with_zeros(vector_t* vector, vector_size_t size);

vector_err_t vector_resize_from_array(vector_t* vector,
                                      vector_size_t size,
                                      vector_elem_t (*array)[size]);

vector_err_t vector_fill_with_elem(vector_t* vector, vector_elem_t elem);

vector_err_t vector_fill_with_zeros(vector_t* vector);

vector_err_t vector_fill_from_array(vector_t* vector,
                                    vector_elem_t (*array)[vector->size]);

vector_err_t vector_copy(vector_t const* source, vector_t* destination);

vector_err_t vector_move(vector_t* source, vector_t* destination);

vector_err_t vector_sum(vector_t const* vector1,
                        vector_t const* vector2,
                        vector_t* sum);

vector_err_t vector_difference(vector_t const* vector1,
                               vector_t const* vector2,
                               vector_t* difference);

vector_err_t vector_scale(vector_t const* vector,
                          vector_elem_t scalar,
                          vector_t* scale);

vector_err_t vector_product(vector_t const* vector1,
                            vector_t const* vector2,
                            vector_elem_t* product);

vector_err_t vector_print(vector_t const* vector);

#endif // LINALG_VECTOR_H