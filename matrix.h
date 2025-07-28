#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include "math.h"
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define MATRIX_INDEX(MATRIX, ROW, COLUMN) \
    ((MATRIX)->data[(ROW) * (MATRIX)->columns + (COLUMN)])

#define MATRIX_RET_ON_ERR(ERR)        \
    do {                              \
        if ((ERR) != MATRIX_ERR_OK) { \
            return (ERR);             \
        }                             \
    } while (0)

typedef enum {
    MATRIX_ERR_OK = 0,
    MATRIX_ERR_FAIL,
    MATRIX_ERR_NULL,
    MATRIX_ERR_BOUNDS,
    MATRIX_ERR_EMPTY,
    MATRIX_ERR_SINGULAR,
    MATRIX_ERR_ALLOC,
    MATRIX_ERR_DIMENSION,
} matrix_err_t;

typedef float matrix_elem_t;
typedef size_t matrix_size_t;

typedef void* (*matrix_allocate_t)(size_t);
typedef void (*matrix_deallocate_t)(void*);
typedef int (*matrix_vprint_t)(char const*, va_list);

typedef struct {
    matrix_vprint_t vprint;
    matrix_allocate_t allocate;
    matrix_deallocate_t deallocate;
} matrix_interface_t;

typedef struct {
    matrix_elem_t* data;
    matrix_size_t rows;
    matrix_size_t columns;
    matrix_interface_t interface;
} matrix_t;

matrix_err_t matrix_initialize(matrix_t* matrix,
                               matrix_interface_t const* interface);

matrix_err_t matrix_deinitialize(matrix_t* matrix);

matrix_size_t matrix_rows(matrix_t const* matrix);

matrix_size_t matrix_columns(matrix_t const* matrix);

matrix_err_t matrix_create(matrix_t* matrix,
                           matrix_size_t rows,
                           matrix_size_t columns);

matrix_err_t matrix_create_with_zeros(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns);

matrix_err_t matrix_create_from_array(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns,
                                      matrix_elem_t (*array)[rows][columns]);

matrix_err_t matrix_delete(matrix_t* matrix);

matrix_err_t matrix_resize(matrix_t* matrix,
                           matrix_size_t rows,
                           matrix_size_t columns);

matrix_err_t matrix_resize_with_zeros(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns);

matrix_err_t matrix_resize_from_array(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns,
                                      matrix_elem_t (*array)[rows][columns]);

matrix_err_t matrix_clear(matrix_t* matrix);

matrix_err_t matrix_print(matrix_t const* matrix);

matrix_err_t matrix_fill_with_zeros(matrix_t* matrix);

matrix_err_t matrix_fill_from_array(
    matrix_t* matrix,
    matrix_elem_t (*array)[matrix->rows][matrix->columns]);

matrix_err_t matrix_copy(matrix_t const* source, matrix_t* destination);

matrix_err_t matrix_move(matrix_t* source, matrix_t* destination);

matrix_err_t matrix_minor(matrix_t const* matrix,
                          size_t row,
                          size_t column,
                          matrix_t* minor);

matrix_err_t matrix_complement(matrix_t const* matrix, matrix_t* complement);

matrix_err_t matrix_adjoint(matrix_t const* matrix, matrix_t* adjoint);

matrix_err_t matrix_transposition(matrix_t const* matrix,
                                  matrix_t* transposition);

matrix_err_t matrix_determinant(matrix_t const* matrix,
                                matrix_elem_t* determinant);

matrix_err_t matrix_inversion(matrix_t const* matrix, matrix_t* inversion);

matrix_err_t matrix_upper_triangular(matrix_t const* matrix,
                                     matrix_t* upper_triangular);

matrix_err_t matrix_lower_triangular(matrix_t const* matrix,
                                     matrix_t* lower_triangular);

matrix_err_t matrix_sum(matrix_t const* matrix1,
                        matrix_t const* matrix2,
                        matrix_t* sum);

matrix_err_t matrix_difference(matrix_t const* matrix1,
                               matrix_t const* matrix2,
                               matrix_t* difference);

matrix_err_t matrix_scale(matrix_t const* matrix,
                          matrix_elem_t scalar,
                          matrix_t* scale);

matrix_err_t matrix_product(matrix_t const* matrix1,
                            matrix_t const* matrix2,
                            matrix_t* product);

matrix_err_t matrix_division(matrix_t const* matrix1,
                             matrix_t const* matrix2,
                             matrix_t* division);

matrix_err_t matrix_power(matrix_t const* matrix,
                          matrix_size_t exponent,
                          matrix_t* power);

matrix_err_t matrix_rank(matrix_t const* matrix, matrix_size_t* rank);

matrix_err_t matrix_eigvals(matrix_t const* matrix,
                            matrix_elem_t** eigvals,
                            matrix_size_t* eigvals_num);

#endif // MATRIX_MATRIX_H