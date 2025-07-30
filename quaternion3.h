#ifndef LINALG_QUATERNION3_H
#define LINALG_QUATERNION3_H

#include <stdint.h>

typedef float quaternion3_data_t;

typedef enum {
    QUATERNION3_ERR_OK = 0,
    QUATERNION3_ERR_FAIL,
    QUATERNION3_ERR_NULL,
} quaternion3_err_t;

typedef int (*quaternion3_print_t)(char const*, ...);

typedef struct {
    quaternion3_data_t w;
    quaternion3_data_t x;
    quaternion3_data_t y;
    quaternion3_data_t z;
} quaternion3_t;

quaternion3_err_t quaternion3_fill_with_zeros(quaternion3_t* quaternion);

quaternion3_err_t quaternion3_fill_with_elements(quaternion3_t* quaternion,
                                                 quaternion3_data_t w,
                                                 quaternion3_data_t x,
                                                 quaternion3_data_t y,
                                                 quaternion3_data_t z);

quaternion3_err_t quaternion3_sum(quaternion3_t const* quaternion1,
                                  quaternion3_t const* quaternion2,
                                  quaternion3_t* sum);

quaternion3_err_t quaternion3_difference(quaternion3_t const* quaternion1,
                                         quaternion3_t const* quaternion2,
                                         quaternion3_t* difference);

quaternion3_err_t quaternion3_hamilton(quaternion3_t const* quaternion1,
                                       quaternion3_t const* quaternion2,
                                       quaternion3_t* hamilton);

quaternion3_err_t quaternion3_scale(quaternion3_t const* quaternion,
                                    quaternion3_data_t scalar,
                                    quaternion3_t* scale);

quaternion3_err_t quaternion3_conjugate(quaternion3_t const* quaternion,
                                        quaternion3_t* conjugate);

quaternion3_err_t quaternion3_inverse(quaternion3_t const* quaternion,
                                      quaternion3_t* inverse);

quaternion3_err_t quaternion3_normalized(quaternion3_t const* quaternion,
                                         quaternion3_t* normalized);

quaternion3_err_t quaternion3_magnitude(quaternion3_t const* quaternion,
                                        quaternion3_data_t* magnitude);

quaternion3_err_t quaternion3_dot(quaternion3_t const* quaternion1,
                                  quaternion3_t const* quaternion2,
                                  quaternion3_data_t* dot);

quaternion3_err_t quaternion3_print(quaternion3_t const* quaternion,
                                    quaternion3_print_t print,
                                    char const* endline);

#endif // LINALG_QUATERNION3_H
