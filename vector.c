#include "vector.h"
#include <string.h>

static void* vector_allocate(vector_t const* vector, size_t size)
{
    if (vector->interface.allocate == NULL) {
        return NULL;
    }

    return vector->interface.allocate(size);
}

static void vector_deallocate(vector_t const* vector, void* data)
{
    if (vector->interface.deallocate == NULL) {
        return;
    }

    vector->interface.deallocate(data);
}

static void vector_vprint(vector_t const* vector, char const* format, ...)
{
    if (vector->interface.vprint == NULL) {
        return;
    }

    va_list args;
    va_start(args, format);
    vector->interface.vprint(format, args);
    va_end(args);
}

vector_err_t vector_initialize(vector_t* vector,
                               vector_interface_t const* interface)
{
    if (vector == NULL || interface == NULL) {
        return VECTOR_ERR_NULL;
    }

    memset(vector, 0, sizeof(*vector));
    memcpy(&vector->interface, interface, sizeof(*interface));

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

    vector_elem_t* data = vector_allocate(vector, sizeof(vector_elem_t) * size);
    if (data == NULL) {
        return VECTOR_ERR_ALLOC;
    }

    vector->data = data;
    vector->size = size;

    return VECTOR_ERR_OK;
}

vector_err_t vector_create_with_elem(vector_t* vector,
                                     vector_size_t size,
                                     vector_elem_t elem)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_create(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_with_elem(vector, elem);
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

vector_err_t vector_create_from_array(vector_t* vector,
                                      vector_size_t size,
                                      vector_elem_t (*array)[size])
{
    if (vector == NULL || array == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_create(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_from_array(vector, array);
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

    vector_elem_t* data = vector_allocate(vector, sizeof(vector_elem_t) * size);
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

vector_err_t vector_resize_with_elem(vector_t* vector,
                                     vector_size_t size,
                                     vector_elem_t elem)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_resize(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_with_elem(vector, elem);
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

vector_err_t vector_resize_from_array(vector_t* vector,
                                      vector_size_t size,
                                      vector_elem_t (*array)[size])
{
    if (vector == NULL || array == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_err_t err = vector_resize(vector, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    return vector_fill_from_array(vector, array);
}

vector_err_t vector_fill_with_elem(vector_t* vector, vector_elem_t elem)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_size_t size = vector->size;

    for (vector_size_t index = 0UL; index < size; ++index) {
        vector->data[index] = elem;
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_fill_with_zeros(vector_t* vector)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_size_t size = vector->size;

    memset(vector->data, 0, sizeof(vector_elem_t) * size);

    return VECTOR_ERR_OK;
}

vector_err_t vector_fill_from_array(vector_t* vector,
                                    vector_elem_t (*array)[vector->size])
{
    if (vector == NULL || array == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_size_t size = vector->size;

    memcpy(vector->data, array, sizeof(vector_elem_t) * size);

    return VECTOR_ERR_OK;
}

vector_err_t vector_copy(vector_t const* source, vector_t* destination)
{
    if (source == NULL || destination == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_size_t size = source->size;

    vector_err_t err = vector_resize(destination, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    memcpy(destination->data, source->data, sizeof(*source->data) * size);

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

    vector_size_t size = vector1->size;

    vector_err_t err = vector_resize(sum, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    for (vector_size_t index = 0UL; index < size; ++index) {
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

    vector_size_t size = vector1->size;

    vector_err_t err = vector_resize(difference, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    for (vector_size_t index = 0UL; index < size; ++index) {
        VECTOR_INDEX(difference, index) =
            VECTOR_INDEX(vector1, index) - VECTOR_INDEX(vector2, index);
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_scale(vector_t const* vector,
                          vector_elem_t scalar,
                          vector_t* scale)
{
    if (vector == NULL || scale == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_size_t size = vector->size;

    vector_err_t err = vector_resize(scale, size);
    if (err != VECTOR_ERR_OK) {
        return err;
    }

    for (vector_size_t index = 0UL; index < size; ++index) {
        VECTOR_INDEX(scale, index) = VECTOR_INDEX(vector, index) * scalar;
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_product(vector_t const* vector1,
                            vector_t const* vector2,
                            vector_elem_t* product)
{
    if (vector1 == NULL || vector2 == NULL || product == NULL) {
        return VECTOR_ERR_NULL;
    }

    if (vector1->size != vector2->size) {
        return VECTOR_ERR_DIMENSION;
    }

    vector_size_t size = vector1->size;

    *product = 0.0F;

    for (vector_size_t index = 0UL; index < size; ++index) {
        *product += VECTOR_INDEX(vector1, index) * VECTOR_INDEX(vector2, index);
    }

    return VECTOR_ERR_OK;
}

vector_err_t vector_print(vector_t const* vector)
{
    if (vector == NULL) {
        return VECTOR_ERR_NULL;
    }

    vector_size_t size = vector->size;

    for (vector_size_t index = 0UL; index < size; ++index) {
        vector_vprint(vector, "%f ", VECTOR_INDEX(vector, index));
    }

    vector_vprint(vector, "\n");

    return VECTOR_ERR_OK;
}
