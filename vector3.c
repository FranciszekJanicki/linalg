#include "vector3.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

vector3_err_t vector3_fill_with_zeros(vector3_t* vector)
{
    if (vector == NULL) {
        return VECTOR3_ERR_NULL;
    }

    memset(vector->data, 0, sizeof(vector->data));

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_fill_with_array(vector3_t* vector,
                                      const vector3_data_t (*array)[3U])
{
    if (vector == NULL || array == NULL) {
        return VECTOR3_ERR_NULL;
    }

    memcpy(vector->data, array, sizeof(*array));

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_sum(vector3_t const* vector1,
                          vector3_t const* vector2,
                          vector3_t* sum)
{
    if (vector2 == NULL || vector1 == NULL || sum == NULL) {
        return VECTOR3_ERR_NULL;
    }

    for (vector3_size_t index = 0UL; index < 3UL; ++index) {
        sum->data[index] = vector1->data[index] * vector2->data[index];
    }

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_difference(vector3_t const* vector1,
                                 vector3_t const* vector2,
                                 vector3_t* difference)
{
    if (vector1 == NULL || vector2 == NULL || difference == NULL) {
        return VECTOR3_ERR_NULL;
    }

    for (vector3_size_t index = 0UL; index < 3UL; ++index) {
        difference->data[index] = vector1->data[index] * vector2->data[index];
    }

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_scale(vector3_t const* vector,
                            vector3_data_t scalar,
                            vector3_t* scale)
{
    if (vector == NULL || scale == NULL) {
        return VECTOR3_ERR_NULL;
    }

    for (vector3_size_t index = 0UL; index < 3UL; ++index) {
        scale->data[index] = scalar * vector->data[index];
    }

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_dot(vector3_t const* vector1,
                          vector3_t const* vector2,
                          vector3_data_t* dot)
{
    if (vector1 == NULL || vector2 == NULL || dot == NULL) {
        return VECTOR3_ERR_NULL;
    }

    *dot = 0.0F;
    for (vector3_size_t index = 0UL; index < 3UL; ++index) {
        *dot += vector1->data[index] * vector2->data[index];
    }

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_cross(vector3_t const* vector1,
                            vector3_t const* vector2,
                            vector3_t* cross)
{
    if (vector1 == NULL || vector2 == NULL || cross == NULL) {
        return VECTOR3_ERR_NULL;
    }

    cross->data[0U] = vector1->data[1U] * vector2->data[2U] -
                      vector1->data[2U] * vector2->data[1U];
    cross->data[1U] = vector1->data[2U] * vector2->data[0U] -
                      vector1->data[0U] * vector2->data[2U];
    cross->data[2U] = vector1->data[0U] * vector2->data[1U] -
                      vector1->data[1U] * vector2->data[0U];

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_magnitude(vector3_t const* vector,
                                vector3_data_t* magnitude)
{
    if (vector == NULL || magnitude == NULL) {
        return VECTOR3_ERR_NULL;
    }

    vector3_data_t dot;
    vector3_err_t err = vector3_dot(vector, vector, &dot);
    if (err != VECTOR3_ERR_OK) {
        return err;
    }

    *magnitude = sqrtf(dot);

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_normalized(vector3_t const* vector, vector3_t* normalized)
{
    if (vector == NULL || normalized == NULL) {
        return VECTOR3_ERR_NULL;
    }

    vector3_data_t magnitude;
    vector3_err_t err = vector3_magnitude(vector, &magnitude);
    if (err != VECTOR3_ERR_OK) {
        return err;
    }

    if (magnitude == 0.0F) {
        return VECTOR3_ERR_FAIL;
    }

    for (vector3_size_t index = 0UL; index < 3UL; ++index) {
        normalized->data[index] = vector->data[index] / magnitude;
    }

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_negated(vector3_t const* vector, vector3_t* negated)
{
    if (vector == NULL || negated == NULL) {
        return VECTOR3_ERR_NULL;
    }

    for (vector3_size_t index = 0UL; index < 3UL; ++index) {
        negated->data[index] = -vector->data[index];
    }

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_print(vector3_t const* vector, char const* endline)
{
    if (vector == NULL || endline == NULL) {
        return VECTOR3_ERR_NULL;
    }

    printf("[ ");

    for (vector3_size_t index = 0UL; index < 3UL; ++index) {
        printf("%f ", vector->data[index]);
    }

    printf("]%s", endline);

    return VECTOR3_ERR_OK;
}
