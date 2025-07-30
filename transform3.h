#ifndef LINALG_TRANSFORM3_H
#define LINALG_TRANSFORM3_H

#include "matrix3.h"
#include "vector3.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TRANSFORM3_ERR_OK = 0,
    TRANSFORM3_ERR_FAIL,
    TRANSFORM3_ERR_NULL,
} transform3_err_t;

typedef int (*transform3_print_t)(char const*, ...);

typedef struct {
    matrix3_t rotation;
    vector3_t translation;
} transform3_t;

transform3_err_t transform3_initialize(transform3_t* transform,
                                       matrix3_t const* matrix,
                                       vector3_t const* vector);

transform3_err_t transform3_deinitialize(transform3_t* transform);

transform3_err_t transform3_compose(transform3_t const* transform1,
                                    transform3_t const* transform2,
                                    transform3_t* compose);

transform3_err_t transform3_inverse(transform3_t const* transform,
                                    transform3_t* inverse);

transform3_err_t transform3_vector_transformation(transform3_t const* transform,
                                                  vector3_t const* vector,
                                                  vector3_t* transformation);

transform3_err_t transform3_print(transform3_t const* transform,
                                  transform3_print_t print,
                                  char const* endline);

#ifdef __cplusplus
}
#endif

#endif // LINALG_TRANSFORM3_H
