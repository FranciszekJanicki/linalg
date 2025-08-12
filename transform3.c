#include "transform3.h"
#include <stdio.h>
#include <string.h>

transform3_err_t transform3_fill_with_arrays(
    transform3_t* transform,
    const matrix3_data_t (*rotation_array)[3U][3U],
    const vector3_data_t (*translation_array)[3U])
{
    if (transform == NULL || rotation_array == NULL ||
        translation_array == NULL) {
        return TRANSFORM3_ERR_NULL;
    }

    if (matrix3_fill_with_array(&transform->rotation, rotation_array) !=
        MATRIX3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    if (vector3_fill_with_array(&transform->translation, translation_array) !=
        VECTOR3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    return TRANSFORM3_ERR_OK;
}

transform3_err_t transform3_fill_with_zeros(transform3_t* transform)
{
    if (transform == NULL) {
        return TRANSFORM3_ERR_NULL;
    }

    if (matrix3_fill_with_zeros(&transform->rotation) != MATRIX3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    if (vector3_fill_with_zeros(&transform->translation) != VECTOR3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    return TRANSFORM3_ERR_OK;
}

transform3_err_t transform3_compose(transform3_t const* transform1,
                                    transform3_t const* transform2,
                                    transform3_t* compose)
{
    if (transform1 == NULL || transform2 == NULL || compose == NULL) {
        return TRANSFORM3_ERR_NULL;
    }

    matrix3_t rotated_rotation;
    if (matrix3_product(&transform1->rotation,
                        &transform2->rotation,
                        &rotated_rotation) != MATRIX3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    vector3_t rotated_translation;
    if (matrix3_vector_product(&transform1->rotation,
                               &transform2->translation,
                               &rotated_translation) != MATRIX3_ERR_OK) {
       return TRANSFORM3_ERR_FAIL;
    }

    vector3_t composed_translation;
    if (vector3_sum(&transform1->translation,
                    &rotated_translation,
                    &composed_translation) != VECTOR3_ERR_OK) {
       return TRANSFORM3_ERR_FAIL;
    }

    compose->rotation = rotated_rotation;
    compose->translation = composed_translation;

    return TRANSFORM3_ERR_OK;
}

transform3_err_t transform3_inverse(transform3_t const* transform,
                                    transform3_t* inverse)
{
    if (transform == NULL || inverse == NULL) {
        return TRANSFORM3_ERR_NULL;
    }

    matrix3_t rotation_inv;
    if (matrix3_inverse(&transform->rotation, &rotation_inv) !=
        MATRIX3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    vector3_t neg_translation;
    if (vector3_negated(&transform->translation, &neg_translation) !=
        VECTOR3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    vector3_t inv_translation;
    if (matrix3_vector_product(&rotation_inv,
                               &neg_translation,
                               &inv_translation) != MATRIX3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    inverse->rotation = rotation_inv;
    inverse->translation = inv_translation;

    return TRANSFORM3_ERR_OK;
}

transform3_err_t transform3_vector_transformation(transform3_t const* transform,
                                                  vector3_t const* vector,
                                                  vector3_t* transformation)
{
    if (transform == NULL || vector == NULL || transformation == NULL) {
        return TRANSFORM3_ERR_NULL;
    }

    if (matrix3_vector_product(&transform->rotation, vector, transformation) !=
        MATRIX3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    if (vector3_sum(transformation, &transform->translation, transformation) !=
        VECTOR3_ERR_OK) {
        return TRANSFORM3_ERR_FAIL;
    }

    return TRANSFORM3_ERR_OK;
}

transform3_err_t transform3_print(transform3_t const* transform,
                                  char const* endline)
{
    if (transform == NULL || endline == NULL) {
        return TRANSFORM3_ERR_NULL;
    }

    printf("Rotation: %s", endline);
    matrix3_print(&transform->rotation, endline);

    printf("Translation: %s", endline);
    vector3_print(&transform->translation, endline);

    return TRANSFORM3_ERR_OK;
}
