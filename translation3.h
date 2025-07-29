#ifndef LINALG_TRANSLATION3_H
#define LINALG_TRANSLATION3_H

#include <stdint.h>

typedef float translation3_data_t;

typedef enum {
    TRANSLATION3_ERR_OK = 0,
    TRANSLATION3_ERR_FAIL,
    TRANSLATION3_ERR_NULL,
} translation3_err_t;

typedef struct {
    translation3_data_t x_data;
    translation3_data_t y_data;
    translation3_data_t z_data;
} translation3_t;

translation3_err_t translation3_initialize(translation3_t* translation3,
                                           translation3_data_t x,
                                           translation3_data_t y,
                                           translation3_data_t z);

translation3_err_t translation3_deinitialize(translation3_t* translation3);

translation3_err_t translation3_sum(translation3_t const* translation3_1,
                                    translation3_t const* translation3_2,
                                    translation3_t* sum);

translation3_err_t translation3_difference(translation3_t const* translation3_1,
                                           translation3_t const* translation3_2,
                                           translation3_t* difference);

translation3_err_t translation3_product(translation3_t const* translation3_1,
                                        translation3_t const* translation3_2,
                                        translation3_t* product);

translation3_err_t translation3_scale(translation3_t const* translation3_1,
                                      translation3_data_t scalar,
                                      translation3_t* scale);

#endif // LINALG_TRANSLATION3_H
