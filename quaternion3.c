#include "quaternion3.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

quaternion3_err_t quaternion3_fill_with_zeros(quaternion3_t* quaternion)
{
    if (quaternion == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    memset(quaternion, 0, sizeof(*quaternion));

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_fill_with_elements(quaternion3_t* quaternion,
                                                 quaternion3_data_t w,
                                                 quaternion3_data_t x,
                                                 quaternion3_data_t y,
                                                 quaternion3_data_t z)
{
    if (quaternion == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    quaternion->w = w;
    quaternion->x = x;
    quaternion->y = y;
    quaternion->z = z;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_sum(quaternion3_t const* quaternion1,
                                  quaternion3_t const* quaternion2,
                                  quaternion3_t* sum)
{
    if (quaternion1 == NULL || quaternion2 == NULL || sum == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    sum->w = quaternion1->w + quaternion2->w;
    sum->x = quaternion1->x + quaternion2->x;
    sum->y = quaternion1->y + quaternion2->y;
    sum->z = quaternion1->z + quaternion2->z;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_difference(quaternion3_t const* quaternion1,
                                         quaternion3_t const* quaternion2,
                                         quaternion3_t* difference)
{
    if (quaternion1 == NULL || quaternion2 == NULL || difference == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    difference->w = quaternion1->w - quaternion2->w;
    difference->x = quaternion1->x - quaternion2->x;
    difference->y = quaternion1->y - quaternion2->y;
    difference->z = quaternion1->z - quaternion2->z;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_hamilton(quaternion3_t const* quaternion1,
                                       quaternion3_t const* quaternion2,
                                       quaternion3_t* hamilton)
{
    if (quaternion1 == NULL || quaternion2 == NULL || hamilton == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    hamilton->w =
        quaternion1->w * quaternion2->w - quaternion1->x * quaternion2->x -
        quaternion1->y * quaternion2->y - quaternion1->z * quaternion2->z;

    hamilton->x =
        quaternion1->w * quaternion2->x + quaternion1->x * quaternion2->w +
        quaternion1->y * quaternion2->z - quaternion1->z * quaternion2->y;

    hamilton->y =
        quaternion1->w * quaternion2->y - quaternion1->x * quaternion2->z +
        quaternion1->y * quaternion2->w + quaternion1->z * quaternion2->x;

    hamilton->z =
        quaternion1->w * quaternion2->z + quaternion1->x * quaternion2->y -
        quaternion1->y * quaternion2->x + quaternion1->z * quaternion2->w;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_scale(quaternion3_t const* quaternion,
                                    quaternion3_data_t scalar,
                                    quaternion3_t* scale)
{
    if (quaternion == NULL || scale == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    scale->w = quaternion->w * scalar;
    scale->x = quaternion->x * scalar;
    scale->y = quaternion->y * scalar;
    scale->z = quaternion->z * scalar;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_conjugate(quaternion3_t const* quaternion,
                                        quaternion3_t* conjugate)
{
    if (quaternion == NULL || conjugate == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    conjugate->w = quaternion->w;
    conjugate->x = -quaternion->x;
    conjugate->y = -quaternion->y;
    conjugate->z = -quaternion->z;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_inverse(quaternion3_t const* quaternion,
                                      quaternion3_t* inverse)
{
    if (quaternion == NULL || inverse == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    quaternion3_data_t mag_sq =
        quaternion->w * quaternion->w + quaternion->x * quaternion->x +
        quaternion->y * quaternion->y + quaternion->z * quaternion->z;

    if (mag_sq == 0.0F) {
        return QUATERNION3_ERR_FAIL;
    }

    quaternion3_t conj;

    quaternion3_conjugate(quaternion, &conj);

    inverse->w = conj.w / mag_sq;
    inverse->x = conj.x / mag_sq;
    inverse->y = conj.y / mag_sq;
    inverse->z = conj.z / mag_sq;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_normalized(quaternion3_t const* quaternion,
                                         quaternion3_t* normalized)
{
    if (quaternion == NULL || normalized == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    quaternion3_data_t mag =
        sqrtf(quaternion->w * quaternion->w + quaternion->x * quaternion->x +
              quaternion->y * quaternion->y + quaternion->z * quaternion->z);

    if (mag == 0) {
        return QUATERNION3_ERR_FAIL;
    }

    normalized->w = quaternion->w / mag;
    normalized->x = quaternion->x / mag;
    normalized->y = quaternion->y / mag;
    normalized->z = quaternion->z / mag;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_magnitude(quaternion3_t const* quaternion,
                                        quaternion3_data_t* magnitude)
{
    if (quaternion == NULL || magnitude == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    *magnitude =
        sqrtf(quaternion->w * quaternion->w + quaternion->x * quaternion->x +
              quaternion->y * quaternion->y + quaternion->z * quaternion->z);

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_dot(quaternion3_t const* quaternion1,
                                  quaternion3_t const* quaternion2,
                                  quaternion3_data_t* dot)
{
    if (quaternion1 == NULL || quaternion2 == NULL || dot == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    *dot = quaternion1->w * quaternion2->w + quaternion1->x * quaternion2->x +
           quaternion1->y * quaternion2->y + quaternion1->z * quaternion2->z;

    return QUATERNION3_ERR_OK;
}

quaternion3_err_t quaternion3_print(quaternion3_t const* quaternion,
                                    quaternion3_print_t print)
{
    if (quaternion == NULL || print == NULL) {
        return QUATERNION3_ERR_NULL;
    }

    print("[ w: %f, x: %f, y: %f, z: %f ]\n\n",
          quaternion->w,
          quaternion->x,
          quaternion->y,
          quaternion->z);

    return QUATERNION3_ERR_OK;
}
