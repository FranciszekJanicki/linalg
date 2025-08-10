#ifndef LINALG_MATRIX3_H
#define LINALG_MATRIX3_H

#include "vector3.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MATRIX3_INDEX(MATRIX, ROW, COLUMN) ((MATRIX)->data[(ROW)][(COLUMN)])

typedef float matrix3_data_t;
typedef size_t matrix3_size_t;

typedef enum {
    MATRIX3_ERR_OK = 0,
    MATRIX3_ERR_FAIL,
    MATRIX3_ERR_NULL,
    MATRIX3_ERR_SINGULAR,
} matrix3_err_t;

typedef struct {
    matrix3_data_t data[3U][3U];
} matrix3_t;

matrix3_err_t matrix3_fill_with_zeros(matrix3_t* matrix);

matrix3_err_t matrix3_fill_with_array(matrix3_t* matrix,
                                      const matrix3_data_t (*array)[3U][3U]);

matrix3_err_t matrix3_minor(matrix3_t const* matrix,
                            matrix3_size_t minor_row,
                            matrix3_size_t minor_column,
                            matrix3_t* minor);

matrix3_err_t matrix3_complement(matrix3_t const* matrix,
                                 matrix3_t* complement);

matrix3_err_t matrix3_adjoint(matrix3_t const* matrix, matrix3_t* adjoint);

matrix3_err_t matrix3_transpose(matrix3_t const* matrix, matrix3_t* transpose);

matrix3_err_t matrix3_det(matrix3_t const* matrix, matrix3_data_t* det);

matrix3_err_t matrix3_inverse(matrix3_t const* matrix, matrix3_t* inverse);

matrix3_err_t matrix3_upper_triangular(matrix3_t const* matrix,
                                       matrix3_t* upper_triangular);

matrix3_err_t matrix3_lower_triangular(matrix3_t const* matrix,
                                       matrix3_t* lower_triangular);

matrix3_err_t matrix3_row_echelon_form(matrix3_t const* matrix,
                                       matrix3_t* row_echelon_form);

matrix3_err_t matrix3_sum(matrix3_t const* matrix1,
                          matrix3_t const* matrix2,
                          matrix3_t* sum);

matrix3_err_t matrix3_difference(matrix3_t const* matrix1,
                                 matrix3_t const* matrix2,
                                 matrix3_t* difference);

matrix3_err_t matrix3_scale(matrix3_t const* matrix,
                            matrix3_data_t scalar,
                            matrix3_t* scale);

matrix3_err_t matrix3_product(matrix3_t const* matrix1,
                              matrix3_t const* matrix2,
                              matrix3_t* product);

matrix3_err_t matrix3_division(matrix3_t const* matrix1,
                               matrix3_t const* matrix2,
                               matrix3_t* division);

matrix3_err_t matrix3_power(matrix3_t const* matrix,
                            matrix3_size_t exponent,
                            matrix3_t* power);

matrix3_err_t matrix3_trace(matrix3_t const* matrix, matrix3_data_t* trace);

matrix3_err_t matrix3_rank(matrix3_t const* matrix, matrix3_size_t* rank);

matrix3_err_t matrix3_eigvals(matrix3_t const* matrix,
                              matrix3_data_t (*eigvals)[3U]);

#include "matrix3.h"

matrix3_err_t matrix3_vector_product(matrix3_t const* matrix,
                                     vector3_t const* vector,
                                     vector3_t* product);

matrix3_err_t matrix3_print(matrix3_t const* matrix, char const* endline);

#ifdef __cplusplus
}
#endif

#endif // LINALG_MATRIX3_H
