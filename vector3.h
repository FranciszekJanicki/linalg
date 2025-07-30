#ifndef LINALG_VECTOR3_H
#define LINALG_VECTOR3_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VECTOR3_INDEX(VECTOR, INDEX) ((VECTOR)->data[(INDEX)])

typedef size_t vector3_size_t;
typedef float vector3_data_t;

typedef int (*vector3_print_t)(char const*, ...);

typedef enum {
    VECTOR3_ERR_OK = 0,
    VECTOR3_ERR_FAIL,
    VECTOR3_ERR_NULL,
} vector3_err_t;

typedef struct {
    vector3_data_t data[3U];
} vector3_t;

vector3_err_t vector3_fill_with_zeros(vector3_t* vector);

vector3_err_t vector3_fill_with_array(vector3_t* vector,
                                      const vector3_data_t (*data)[3U]);

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

vector3_err_t vector3_print(vector3_t const* vector,
                            vector3_print_t print,
                            char const* endline);

#ifdef __cplusplus
}
#endif

#endif // LINALG_VECTOR3_H
