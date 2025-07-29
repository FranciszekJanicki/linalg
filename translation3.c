#include "translation3.h"
#include <math.h>
#include <string.h>

translation3_err_t translation3_initialize(translation3_t* vector,
                                           translation3_data_t x,
                                           translation3_data_t y,
                                           translation3_data_t z)
{
    if (vector == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    vector->x = x;
    vector->y = y;
    vector->z = z;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_sum(translation3_t const* left,
                                    translation3_t const* right,
                                    translation3_t* result)
{
    if (left == NULL || right == NULL || result == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    result->x = left->x + right->x;
    result->y = left->y + right->y;
    result->z = left->z + right->z;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_difference(translation3_t const* left,
                                           translation3_t const* right,
                                           translation3_t* result)
{
    if (left == NULL || right == NULL || result == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    result->x = left->x - right->x;
    result->y = left->y - right->y;
    result->z = left->z - right->z;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_scale(translation3_t const* vector,
                                      translation3_data_t scalar,
                                      translation3_t* result)
{
    if (vector == NULL || result == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    result->x = vector->x * scalar;
    result->y = vector->y * scalar;
    result->z = vector->z * scalar;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_dot(translation3_t const* left,
                                    translation3_t const* right,
                                    translation3_data_t* result)
{
    if (left == NULL || right == NULL || result == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    *result = left->x * right->x + left->y * right->y + left->z * right->z;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_cross(translation3_t const* left,
                                      translation3_t const* right,
                                      translation3_t* result)
{
    if (left == NULL || right == NULL || result == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    result->x = left->y * right->z - left->z * right->y;
    result->y = left->z * right->x - left->x * right->z;
    result->z = left->x * right->y - left->y * right->x;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_magnitude(translation3_t const* vector,
                                          translation3_data_t* magnitude)
{
    if (vector == NULL || magnitude == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    *magnitude = sqrtf(vector->x * vector->x + vector->y * vector->y +
                       vector->z * vector->z);

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_normalized(translation3_t const* vector,
                                           translation3_t* result)
{
    if (vector == NULL || result == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    translation3_data_t mag = 0.0f;
    translation3_err_t err = translation3_magnitude(vector, &mag);
    if (err != TRANSLATION3_ERR_OK) {
        return err;
    }

    if (mag == 0.0f) {
        // Cannot normalize zero vector
        return TRANSLATION3_ERR_FAIL;
    }

    result->x = vector->x / mag;
    result->y = vector->y / mag;
    result->z = vector->z / mag;

    return TRANSLATION3_ERR_OK;
}
