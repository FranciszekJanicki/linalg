#ifndef LINALG_ROTATION3_H
#define LINALG_ROTATION3_H

#include "quaternion3.h"
#include "translation3.h"
#include <stdint.h>

typedef float rotation3_data_t;

typedef enum {
    ROTATION3_ERR_OK = 0,
    ROTATION3_ERR_FAIL,
    ROTATION3_ERR_NULL,
} rotation3_err_t;

typedef struct {
    rotation3_data_t xx, xy, xz;
    rotation3_data_t yx, yy, yz;
    rotation3_data_t zx, zy, zz;
} rotation3_t;

rotation3_err_t rotation3_initialize(rotation3_t* rotation,
                                     rotation3_data_t xx,
                                     rotation3_data_t xy,
                                     rotation3_data_t xz,
                                     rotation3_data_t yx,
                                     rotation3_data_t yy,
                                     rotation3_data_t yz,
                                     rotation3_data_t zx,
                                     rotation3_data_t zy,
                                     rotation3_data_t zz);

rotation3_err_t rotation3_product(rotation3_t const* left,
                                  rotation3_t const* right,
                                  rotation3_t* result);

rotation3_err_t rotation3_transpose(rotation3_t const* rotation,
                                    rotation3_t* result);

rotation3_err_t rotation3_apply_to_vector(rotation3_t const* rotation,
                                          translation3_t const* vector,
                                          translation3_t* result);

rotation3_err_t rotation3_from_quaternion(quaternion3_t const* quaternion,
                                          rotation3_t* result);

rotation3_err_t rotation3_to_quaternion(rotation3_t const* rotation,
                                        quaternion3_t* result);

#endif // LINALG_ROTATION3_H
