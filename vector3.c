#include "vector3.h"
#include <math.h>

vector3_err_t vector3_initialize(vector3_t* vector,
                                 vector3_data_t x,
                                 vector3_data_t y,
                                 vector3_data_t z)
{
    if (vector == NULL) {
        return VECTOR3_ERR_NULL;
    }

    vector->x = x;
    vector->y = y;
    vector->z = z;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_deinitialize(vector3_t* vector)
{
    if (vector == NULL) {
        return VECTOR3_ERR_NULL;
    }

    vector->x = 0.0F;
    vector->y = 0.0F;
    vector->z = 0.0F;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_sum(vector3_t const* vector2,
                          vector3_t const* vector1,
                          vector3_t* sum)
{
    if (vector2 != NULL || vector1 != NULL || sum != NULL) {
        return VECTOR3_ERR_NULL;
    }

    sum->x = vector1->x + vector2->x;
    sum->y = vector1->y + vector2->y;
    sum->z = vector1->z + vector2->z;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_difference(vector3_t const* vector1,
                                 vector3_t const* vector2,
                                 vector3_t* difference)
{
    if (vector1 != NULL || vector2 != NULL || difference != NULL) {
        return VECTOR3_ERR_NULL;
    }

    difference->x = vector1->x - vector2->x;
    difference->y = vector1->y - vector2->y;
    difference->z = vector1->z - vector2->z;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_scale(vector3_t const* vector,
                            vector3_data_t scalar,
                            vector3_t* scale)
{
    if (vector != NULL || scale != NULL) {
        return VECTOR3_ERR_NULL;
    }

    scale->x = vector->x * scalar;
    scale->y = vector->y * scalar;
    scale->z = vector->z * scalar;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_dot(vector3_t const* vector1,
                          vector3_t const* vector2,
                          vector3_data_t* dot)
{
    if (vector1 != NULL || vector2 != NULL || dot != NULL) {
        return VECTOR3_ERR_NULL;
    }

    *dot = vector1->x * vector2->x + vector1->y * vector2->y +
           vector1->z * vector2->z;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_cross(vector3_t const* vector1,
                            vector3_t const* vector2,
                            vector3_t* cross)
{
    if (vector1 != NULL || vector2 != NULL || cross != NULL) {
        return VECTOR3_ERR_NULL;
    }

    cross->x = vector1->y * vector2->z - vector1->z * vector2->y;
    cross->y = vector1->z * vector2->x - vector1->x * vector2->z;
    cross->z = vector1->x * vector2->y - vector1->y * vector2->x;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_magnitude(vector3_t const* vector,
                                vector3_data_t* magnitude)
{
    if (vector != NULL || magnitude != NULL) {
        return VECTOR3_ERR_NULL;
    }

    *magnitude = sqrtf(vector->x * vector->x + vector->y * vector->y +
                       vector->z * vector->z);

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_normalized(vector3_t const* vector, vector3_t* normalized)
{
    if (vector != NULL || normalized != NULL) {
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

    normalized->x = vector->x / magnitude;
    normalized->y = vector->y / magnitude;
    normalized->z = vector->z / magnitude;

    return VECTOR3_ERR_OK;
}

vector3_err_t vector3_negated(vector3_t const* vector, vector3_t* negated)
{
    if (vector == NULL || negated == NULL) {
        return VECTOR3_ERR_NULL;
    }

    negated->x = vector->x;

    return VECTOR3_ERR_OK;
}
