#ifndef LINALG_VECTOR3_H
#define LINALG_VECTOR3_H

#include <stdint.h>

typedef float vector3_data_t;

typedef enum {
    VECTOR3_ERR_OK = 0,
    VECTOR3_ERR_FAIL,
    VECTOR3_ERR_NULL,
} vector3_err_t;

typedef struct {
    vector3_data_t x;
    vector3_data_t y;
    vector3_data_t z;
} vector3_t;

vector3_err_t vector3_initialize(vector3_t* vector,
                                 vector3_data_t x,
                                 vector3_data_t y,
                                 vector3_data_t z);

vector3_err_t vector3_deinitialize(vector3_t* vector);

vector3_err_t vector3_sum(vector3_t const* vector1,
                          vector3_t const* vector2,
                          vector3_t* sum);

vector3_err_t vector3_difference(vector3_t const* vector1,
                                 vector3_t const* vector2,
                                 vector3_t* difference);

vector3_err_t vector3_scale(vector3_t const* vector,
                            vector3_data_t scalar,
                            vector3_t* scale);

vector3_err_t vector3_dot(vector3_t const* vector1,
                          vector3_t const* vector2,
                          vector3_data_t* dot);

vector3_err_t vector3_cross(vector3_t const* vector1,
                            vector3_t const* vector2,
                            vector3_t* cross);

vector3_err_t vector3_normalized(vector3_t const* vector,
                                 vector3_t* normalized);

vector3_err_t vector3_magnitude(vector3_t const* vector,
                                vector3_data_t* magnitude);

vector3_err_t vector3_negated(vector3_t const* vector, vector3_t* negated);

#endif // LINALG_VECTOR3_H
