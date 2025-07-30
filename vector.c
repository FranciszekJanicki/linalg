#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static void* vector_allocate(vector_t const* vector, size_t size)
{
    if (vector->allocator.allocate == NULL) {
        return NULL;
    }

    return vector->allocator.allocate(size);
}

static void vector_deallocate(vector_t const* vector, void* data)
{
    if (vector->allocator.deallocate == NULL) {
        return;
    }

    vector->allocator.deallocate(data);
}

vector_err_t vector_initialize(vector_t* vector,
                               vector_allocator_t const* allocator)
{
    if (vector == NULL || allocator == NULL) {
        return VECTOR_ERR_NULL;
    }

    memset(vector, 0, sizeof(*vector));
    memcpy(&vector->allocator, allocator, sizeof(*allocator));

    return VECTOR_ERR_OK;
}

vector_err_t vector_deinitialize(vector_t* vector)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    memset(vector, 0, sizeof(*vector));

    return VECTOR_ERR_OK;
}

vector_size_t vector_size(vector_t const* vector)
{
    if (vector == NULL) {
        return 0UL;
    }

    return vector->size;
}

vector_err_t vector_create(vector_t* vector, vector_size_t size)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_data_t* data = vector_allocate(vector, sizeof(vector_data_t) * size);
    if (data == NULL) {
        return VECTOR_ERR_ALLOC;
    }

    vector->data = data;
    vector->size = size;

    return VECTOR_ERR_OK;
}

vector_err_t vector_create_with_zeros(vector_t* vector, vector_size_t size)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_create(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_with_zeros(vector);
}

vector_err_t vector_create_with_array(vector_t* vector,
                                      vector_size_t size,
                                      const vector_data_t (*array)[size])
{
    if (vector == NULL || array == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_create(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_with_array(vector, array);
}

vector_err_t vector_delete(vector_t* vector)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector->data != NULL) {
        vector_deallocate(vector, vector->data);
    }

    vector->data = NULL;
    vector->size = 0UL;

    return VECTOR_ERR_OK;
}

vector_err_t vector_resize(vector_t* vector, vector_size_t size)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector->data != NULL && vector->size == size) {
        return VECTOR_ERR_OK;
    }

    vector_data_t* data = vector_allocate(vector, sizeof(vector_data_t) * size);
    if (data == NULL) {
        return VECTOR_ERR_ALLOC;
    }

    if (vector->data != NULL) {
        vector_deallocate(vector, vector->data);
    }

    vector->data = data;
    vector->size = size;

    return VECTOR_ERR_OK;
}

vector_err_t vector_resize_with_zeros(vector_t* vector, vector_size_t size)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_resize(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_with_zeros(vector);
}

vector_err_t vector_resize_with_array(vector_t* vector,
                                      vector_size_t size,
                                      const vector_data_t (*array)[size])
{
    if (vector == NULL || array == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_resize(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_with_array(vector, array);
}

vector_err_t vector_fill_with_zeros(vector_t* vector)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    memset(vector->data, 0, sizeof(vector_data_t) * vector->size);

    return VECTOR_ERR_OK;
}

vector_err_t vector_fill_with_array(vector_t* vector,
                                    const vector_data_t (*array)[vector->size])
{
    if (vector == NULL || array == NULL) {
        return VECTOR_ERR_NULL;
    }

    memcpy(vector->data, array, sizeof(vector_data_t) * vector->size);

    return VECTOR_ERR_OK;
}

vector_err_t vector_copy(vector_t const* source, vector_t* destination)
{
    if (source == NULL || destination == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_resize(destination, source->size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    memcpy(destination->data,
           source->data,
           sizeof(*source->data) * source->size);

    return VECTOR_ERR_OK;
}

vector_err_t vector_move(vector_t* source, vector_t* destination)
{
    if (source == NULL || destination == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_delete(destination);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    destination->data = source->data;
    destination->size = source->size;

    source->data = NULL;
    source->size = 0UL;

    return VECTOR_ERR_OK;
}

vector_err_t vector_sum(vector_t const* vector1,
                        vector_t const* vector2,
                        vector_t* sum)
{
    if (vector1 == NULL || vector2 == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector1->size != vector2->size) {
        return VECTOR_ERR_DIMENSION;
    }

    vector_err_t err = vector_resize(sum, vector1->size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    for (vector_size_t index = 0UL; index < vector1->size; ++index) {
        VECTOR_INDEX(sum, index) =
            VECTOR_INDEX(vector1, index) + VECTOR_INDEX(vector2, index);
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_difference(vector_t const* vector1,
                               vector_t const* vector2,
                               vector_t* difference)
{
    if (vector1 == NULL || vector2 == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector1->size != vector2->size) {
        return VECTOR_ERR_DIMENSION;
    }

    vector_err_t err = vector_resize(difference, vector1->size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    for (vector_size_t index = 0UL; index < vector1->size; ++index) {
        VECTOR_INDEX(difference, index) =
            VECTOR_INDEX(vector1, index) - VECTOR_INDEX(vector2, index);
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_scale(vector_t const* vector,
                          vector_data_t scalar,
                          vector_t* scale)
{
    if (vector == NULL || scale == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_resize(scale, vector->size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    for (vector_size_t index = 0UL; index < vector->size; ++index) {
        VECTOR_INDEX(scale, index) = VECTOR_INDEX(vector, index) * scalar;
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_dot(vector_t const* vector1,
                        vector_t const* vector2,
                        vector_data_t* dot)
{
    if (vector1 == NULL || vector2 == NULL || dot == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector1->size != vector2->size) {
        return VECTOR_ERR_DIMENSION;
    }

    *dot = 0.0F;

    for (vector_size_t index = 0UL; index < vector1->size; ++index) {
        *dot += VECTOR_INDEX(vector1, index) * VECTOR_INDEX(vector2, index);
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_cross(vector_t const* vector1,
                          vector_t const* vector2,
                          vector_t* cross)
{
    if (vector1 == NULL || vector2 == NULL || cross == NULL) {
    }

    if (vector1->size != 3UL || vector2->size != 3UL) {
        return VECTOR_ERR_DIMENSION;
    }

    vector_err_t err = vector_resize(cross, 3UL);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    VECTOR_INDEX(cross, 0) =
        VECTOR_INDEX(vector1, 1) * VECTOR_INDEX(vector2, 2) -
        VECTOR_INDEX(vector1, 2) * VECTOR_INDEX(vector2, 1);
    VECTOR_INDEX(cross, 1) =
        VECTOR_INDEX(vector1, 2) * VECTOR_INDEX(vector2, 0) -
        VECTOR_INDEX(vector1, 0) * VECTOR_INDEX(vector2, 2);
    VECTOR_INDEX(cross, 2) =
        VECTOR_INDEX(vector1, 0) * VECTOR_INDEX(vector2, 1) -
        VECTOR_INDEX(vector1, 1) * VECTOR_INDEX(vector2, 0);

    return VECTOR_ERR_OK;
}

vector_err_t vector_print(vector_t const* vector,
                          vector_print_t print,
                          char const* endline)
{
    if (vector == NULL || endline == NULL) {
        return VECTOR_ERR_NULL;
    }

    print("[ ");

    for (vector_size_t index = 0UL; index < vector->size; ++index) {
        print("%f ", VECTOR_INDEX(vector, index));
    }

    print("]%s", endline);

    return VECTOR_ERR_OK;
}
