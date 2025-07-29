#include "translation3.h"
#include <string.h>

translation3_err_t translation3_initialize(translation3_t* translation3,
                                           translation3_data_t x_data,
                                           translation3_data_t y_data,
                                           translation3_data_t z_data)
{
    if (translation3 == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    memset(translation3, 0, sizeof(*translation3));

    translation3->x_data = x_data;
    translation3->y_data = y_data;
    translation3->z_data = z_data;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_deinitialize(translation3_t* translation3)
{
    if (translation3 == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    memset(translation3, 0, sizeof(*translation3));

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_sum(translation3_t const* translation3_1,
                                    translation3_t const* translation3_2,
                                    translation3_t* sum)
{
    if (translation3_1 == NULL || translation3_2 == NULL || sum == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    sum->x_data = translation3_1->x_data + translation3_2->x_data;
    sum->y_data = translation3_1->y_data + translation3_2->y_data;
    sum->z_data = translation3_1->z_data + translation3_2->z_data;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_difference(translation3_t const* translation3_1,
                                           translation3_t const* translation3_2,
                                           translation3_t* difference)
{
    if (translation3_1 == NULL || translation3_2 == NULL ||
        difference == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    difference->x_data = translation3_1->x_data - translation3_2->x_data;
    difference->y_data = translation3_1->y_data - translation3_2->y_data;
    difference->z_data = translation3_1->z_data - translation3_2->z_data;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_product(translation3_t const* translation3_1,
                                        translation3_t const* translation3_2,
                                        translation3_data_t* product)
{
    if (translation3_1 == NULL || translation3_2 == NULL || product == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    *product = translation3_1->x_data * translation3_2->x_data +
               translation3_1->y_data * translation3_2->y_data +
               translation3_1->z_data * translation3_2->z_data;

    return TRANSLATION3_ERR_OK;
}

translation3_err_t translation3_scale(translation3_t const* translation3,
                                      translation3_data_t scalar,
                                      translation3_t* scale)
{
    if (translation3 == NULL || scale == NULL) {
        return TRANSLATION3_ERR_NULL;
    }

    scale->x_data = translation3->x_data * scalar;
    scale->y_data = translation3->y_data * scalar;
    scale->z_data = translation3->z_data * scalar;
}
