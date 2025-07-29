#ifndef LINALG_TRANSFORMATION3_H
#define LINALG_TRANSFORMATION3_H

#include "rotation3.h"
#include "translation3.h"
#include <stdint.h>

typedef float transformation3_data_t;

typedef enum {
    TRANSFORMATION3_ERR_OK = 0,
    TRANSFORMATION3_ERR_FAIL,
    TRANSFORMATION3_ERR_NULL,
} transformation3_err_t;

typedef struct {
    rotation3_t rotation;
    translation3_t translation;
} transformation3_t;

transformation3_err_t transformation3_initialize(
    transformation3_t* transformation,
    rotation3_t const* rotation,
    translation3_t const* translation);

transformation3_err_t transformation3_identity(
    transformation3_t* transformation);

transformation3_err_t transformation3_compose(transformation3_t const* first,
                                              transformation3_t const* second,
                                              transformation3_t* result);

transformation3_err_t transformation3_invert(
    transformation3_t const* transformation,
    transformation3_t* result);

transformation3_err_t transformation3_apply_to_point(
    transformation3_t const* transformation,
    translation3_t const* point,
    translation3_t* result);

#endif // LINALG_TRANSFORMATION3_H
