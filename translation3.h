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
    translation3_data_t x;
    translation3_data_t y;
    translation3_data_t z;
} translation3_t;

translation3_err_t translation3_initialize(translation3_t* vector,
                                           translation3_data_t x,
                                           translation3_data_t y,
                                           translation3_data_t z);

translation3_err_t translation3_sum(translation3_t const* left,
                                    translation3_t const* right,
                                    translation3_t* result);

translation3_err_t translation3_difference(translation3_t const* left,
                                           translation3_t const* right,
                                           translation3_t* result);

translation3_err_t translation3_scale(translation3_t const* vector,
                                      translation3_data_t scalar,
                                      translation3_t* result);

translation3_err_t translation3_dot(translation3_t const* left,
                                    translation3_t const* right,
                                    translation3_data_t* result);

translation3_err_t translation3_cross(translation3_t const* left,
                                      translation3_t const* right,
                                      translation3_t* result);

translation3_err_t translation3_normalized(translation3_t const* vector,
                                           translation3_t* result);

translation3_err_t translation3_magnitude(translation3_t const* vector,
                                          translation3_data_t* magnitude);

#endif // LINALG_TRANSLATION3_H
