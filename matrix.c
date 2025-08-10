#include "matrix.h"
#include <stdio.h>
#include <string.h>

static matrix_data_t* matrix_allocate(matrix_t const* matrix,
                                      matrix_size_t size)
{
    if (matrix->allocator.allocate == NULL) {
        return NULL;
    }

    return matrix->allocator.allocate(matrix->allocator.user, size);
}

static void matrix_deallocate(matrix_t const* matrix, matrix_data_t* data)
{
    if (matrix->allocator.deallocate == NULL) {
        return;
    }

    matrix->allocator.deallocate(matrix->allocator.user, data);
}

matrix_err_t matrix_initialize(matrix_t* matrix,
                               matrix_allocator_t const* allocator)
{
    if (matrix == NULL || allocator == NULL) {
        return MATRIX_ERR_NULL;
    }

    memset(matrix, 0, sizeof(*matrix));
    memcpy(&matrix->allocator, allocator, sizeof(*allocator));

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_deinitialize(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    memset(matrix, 0, sizeof(*matrix));

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_create(matrix_t* matrix,
                           matrix_size_t rows,
                           matrix_size_t columns)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_data_t* data =
        matrix_allocate(matrix, sizeof(matrix_data_t) * rows * columns);
    if (data == NULL) {
        return MATRIX_ERR_ALLOC;
    }

    matrix->data = data;
    matrix->rows = rows;
    matrix->columns = columns;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_create_with_zeros(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_create(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_with_zeros(matrix);
}

matrix_err_t matrix_create_with_array(
    matrix_t* matrix,
    matrix_size_t rows,
    matrix_size_t columns,
    const matrix_data_t (*array)[rows][columns])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_create(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_with_array(matrix, array);
}

matrix_err_t matrix_delete(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->data != NULL) {
        matrix_deallocate(matrix, matrix->data);
    }

    matrix->data = NULL;
    matrix->rows = 0UL;
    matrix->columns = 0UL;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_resize(matrix_t* matrix,
                           matrix_size_t rows,
                           matrix_size_t columns)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->data != NULL && matrix->rows == rows &&
        matrix->columns == columns) {
        return MATRIX_ERR_OK;
    }

    if (matrix->data != NULL && matrix->columns == rows &&
        matrix->rows == columns) {
        matrix->rows = columns;
        matrix->columns = rows;

        return MATRIX_ERR_OK;
    }

    matrix_data_t* data =
        matrix_allocate(matrix, sizeof(matrix_data_t) * rows * columns);
    if (data == NULL) {
        return MATRIX_ERR_ALLOC;
    }

    if (matrix->data != NULL) {
        matrix_deallocate(matrix, matrix->data);
    }

    matrix->data = data;
    matrix->rows = rows;
    matrix->columns = columns;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_resize_with_zeros(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_resize(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_with_zeros(matrix);
}

matrix_err_t matrix_resize_with_array(
    matrix_t* matrix,
    matrix_size_t rows,
    matrix_size_t columns,
    const matrix_data_t (*array)[rows][columns])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_resize(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_with_array(matrix, array);
}

matrix_err_t matrix_fill_with_zeros(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    memset(matrix->data,
           0,
           sizeof(matrix_data_t) * matrix->rows * matrix->columns);

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_fill_with_array(
    matrix_t* matrix,
    const matrix_data_t (*array)[matrix->rows][matrix->columns])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX_ERR_NULL;
    }

    memcpy(matrix->data,
           array,
           sizeof(matrix_data_t) * matrix->rows * matrix->columns);

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_copy(matrix_t const* source, matrix_t* destination)
{
    if (source == NULL || destination == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err =
        matrix_resize(destination, source->rows, source->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    memcpy(destination->data,
           source->data,
           sizeof(*source->data) * source->rows * source->columns);

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_move(matrix_t* source, matrix_t* destination)
{
    if (source == NULL || destination == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_delete(destination);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    destination->data = source->data;
    destination->rows = source->rows;
    destination->columns = source->columns;

    source->data = NULL;
    source->rows = 0UL;
    source->columns = 0UL;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_minor(matrix_t const* matrix,
                          size_t minor_row,
                          size_t minor_column,
                          matrix_t* minor)
{
    if (matrix == NULL || minor == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns || minor_row >= matrix->rows ||
        minor_column >= matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err =
        matrix_resize(minor, matrix->rows - 1UL, matrix->columns - 1UL);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL, cof_row = 0UL; row < matrix->rows; ++row) {
        if (row == minor_row) {
            continue;
        }

        for (matrix_size_t column = 0UL, cof_column = 0UL;
             column < matrix->columns;
             ++column) {
            if (column == minor_column) {
                continue;
            }

            MATRIX_INDEX(minor, cof_row, cof_column) =
                MATRIX_INDEX(matrix, row, column);

            cof_column++;
        }

        cof_row++;
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_complement(matrix_t const* matrix, matrix_t* complement)
{
    if (matrix == NULL || complement == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err = matrix_resize(complement, matrix->rows, matrix->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    matrix_t minor;
    err = matrix_initialize(&minor, &complement->allocator);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < matrix->rows; ++row) {
        for (matrix_size_t column = 0UL; column < matrix->columns; ++column) {
            err = matrix_minor(matrix, row, column, &minor);
            if (err != MATRIX_ERR_OK) {
                matrix_delete(&minor);
                return err;
            }

            matrix_data_t minor_det;
            err = matrix_det(&minor, &minor_det);
            if (err != MATRIX_ERR_OK) {
                matrix_delete(&minor);
                return err;
            }

            MATRIX_INDEX(complement, row, column) =
                (((row + column) & 1UL) ? -1.0F : 1.0F) * minor_det;
        }
    }

    return matrix_delete(&minor);
}

matrix_err_t matrix_adjoint(matrix_t const* matrix, matrix_t* adjoint)
{
    if (matrix == NULL || adjoint == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_t complement;
    matrix_err_t err = matrix_initialize(&complement, &adjoint->allocator);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_complement(matrix, &complement);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_transpose(&complement, adjoint);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_delete(&complement);
}

matrix_err_t matrix_transpose(matrix_t const* matrix, matrix_t* transpose)
{
    if (matrix == NULL || transpose == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_resize(transpose, matrix->columns, matrix->rows);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < matrix->rows; ++row) {
        for (matrix_size_t column = 0UL; column < matrix->columns; ++column) {
            MATRIX_INDEX(transpose, column, row) =
                MATRIX_INDEX(matrix, row, column);
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_det(matrix_t const* matrix, matrix_data_t* det)
{
    if (matrix == NULL || det == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    if (matrix->rows == 1UL) {
        *det = MATRIX_INDEX(matrix, 0UL, 0UL);

        return MATRIX_ERR_OK;
    } else if (matrix->rows == 2UL) {
        *det = MATRIX_INDEX(matrix, 0UL, 0UL) * MATRIX_INDEX(matrix, 1UL, 1UL) -
               MATRIX_INDEX(matrix, 0UL, 1UL) * MATRIX_INDEX(matrix, 1UL, 0UL);

        return MATRIX_ERR_OK;
    }

    matrix_t minor;
    matrix_err_t err = matrix_initialize(&minor, &matrix->allocator);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    *det = 0.0F;
    for (matrix_size_t column = 0UL; column < matrix->columns; ++column) {
        err = matrix_minor(matrix, 0UL, column, &minor);
        if (err != MATRIX_ERR_OK) {
            return err;
        }

        matrix_data_t minor_det;
        err = matrix_det(&minor, &minor_det);
        if (err != MATRIX_ERR_OK) {
            return err;
        }

        *det += ((column & 1U) ? -1.0F : 1.0F) *
                MATRIX_INDEX(matrix, 0UL, column) * minor_det;
    }

    return matrix_delete(&minor);
}

matrix_err_t matrix_inverse(matrix_t const* matrix, matrix_t* inverse)
{
    if (matrix == NULL || inverse == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_t adjoint;
    matrix_err_t err = matrix_initialize(&adjoint, &inverse->allocator);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_adjoint(matrix, &adjoint);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    matrix_data_t det;
    err = matrix_det(matrix, &det);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    if (fabsf(det) < 1E-6F) {
        return MATRIX_ERR_SINGULAR;
    }

    err = matrix_scale(&adjoint, 1.0F / det, inverse);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_delete(&adjoint);
}

matrix_err_t matrix_upper_triangular(matrix_t const* matrix,
                                     matrix_t* upper_triangular)
{
    if (matrix == NULL || upper_triangular == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_t lower_triangular;
    matrix_err_t err =
        matrix_initialize(&lower_triangular, &upper_triangular->allocator);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_lower_triangular(matrix, &lower_triangular);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_transpose(&lower_triangular, upper_triangular);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_delete(&lower_triangular);
}

matrix_err_t matrix_lower_triangular(matrix_t const* matrix,
                                     matrix_t* lower_triangular)
{
    if (matrix == NULL || lower_triangular == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err =
        matrix_resize(lower_triangular, matrix->rows, matrix->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < matrix->rows; ++row) {
        for (matrix_size_t column = 0UL; column <= row; ++column) {
            matrix_data_t sum = 0.0F;

            for (matrix_size_t lower = 0UL; lower < column; ++lower) {
                sum += MATRIX_INDEX(lower_triangular, row, lower) *
                       MATRIX_INDEX(lower_triangular, column, lower);
            }

            if (row == column) {
                matrix_data_t val = MATRIX_INDEX(matrix, row, row) - sum;
                if (val <= 0.0F) {
                    return MATRIX_ERR_DIMENSION;
                }

                MATRIX_INDEX(lower_triangular, row, column) = sqrtf(val);
            } else {
                MATRIX_INDEX(lower_triangular, row, column) =
                    (1.0F / MATRIX_INDEX(lower_triangular, column, column)) *
                    (MATRIX_INDEX(matrix, row, column) - sum);
            }
        }

        for (matrix_size_t column = row + 1UL; column < matrix->rows;
             ++column) {
            MATRIX_INDEX(lower_triangular, row, column) = 0.0F;
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_row_echelon_form(matrix_t const* matrix,
                                     matrix_t* row_echelon_form)
{
    if (matrix == NULL || row_echelon_form == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_copy(matrix, row_echelon_form);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t pivot = 0UL; pivot < 3UL; ++pivot) {
        matrix_size_t max_row = pivot;
        for (matrix_size_t row = pivot + 1UL; row < 3UL; ++row) {
            if (fabsf(MATRIX_INDEX(row_echelon_form, row, pivot)) >
                fabsf(MATRIX_INDEX(row_echelon_form, max_row, pivot))) {
                max_row = row;
            }
        }

        if (fabsf(MATRIX_INDEX(row_echelon_form, max_row, pivot)) < 1E-6F) {
            continue;
        }

        if (max_row != pivot) {
            for (matrix_size_t column = 0UL; column < 3UL; ++column) {
                matrix_data_t temp =
                    MATRIX_INDEX(row_echelon_form, pivot, column);
                MATRIX_INDEX(row_echelon_form, pivot, column) =
                    MATRIX_INDEX(row_echelon_form, max_row, column);
                MATRIX_INDEX(row_echelon_form, max_row, column) = temp;
            }
        }

        for (matrix_size_t row = pivot + 1UL; row < 3UL; ++row) {
            matrix_data_t factor = MATRIX_INDEX(row_echelon_form, row, pivot) /
                                   MATRIX_INDEX(row_echelon_form, pivot, pivot);
            for (matrix_size_t col = pivot; col < 3UL; ++col) {
                MATRIX_INDEX(row_echelon_form, row, col) -=
                    factor * MATRIX_INDEX(row_echelon_form, pivot, col);
            }
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_sum(matrix_t const* matrix1,
                        matrix_t const* matrix2,
                        matrix_t* sum)
{
    if (matrix1 == NULL || matrix2 == NULL || sum == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix1->rows != matrix2->rows ||
        matrix1->columns != matrix2->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err = matrix_resize(sum, matrix1->rows, matrix1->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < matrix1->rows; ++row) {
        for (matrix_size_t column = 0UL; column < matrix2->columns; ++column) {
            MATRIX_INDEX(sum, row, column) =
                MATRIX_INDEX(matrix1, row, column) +
                MATRIX_INDEX(matrix2, row, column);
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_difference(matrix_t const* matrix1,
                               matrix_t const* matrix2,
                               matrix_t* difference)
{
    if (matrix1 == NULL || matrix2 == NULL || difference == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix1->rows != matrix2->rows ||
        matrix1->columns != matrix2->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err =
        matrix_resize(difference, matrix1->rows, matrix1->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < matrix1->rows; ++row) {
        for (matrix_size_t column = 0UL; column < matrix1->columns; ++column) {
            MATRIX_INDEX(difference, row, column) =
                MATRIX_INDEX(matrix1, row, column) -
                MATRIX_INDEX(matrix2, row, column);
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_scale(matrix_t const* matrix,
                          matrix_data_t scalar,
                          matrix_t* scale)
{
    if (matrix == NULL || scale == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_resize(scale, matrix->rows, matrix->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < matrix->rows; ++row) {
        for (matrix_size_t column = 0UL; column < matrix->columns; ++column) {
            MATRIX_INDEX(scale, row, column) =
                scalar * MATRIX_INDEX(matrix, row, column);
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_product(matrix_t const* matrix1,
                            matrix_t const* matrix2,
                            matrix_t* product)
{
    if (matrix1 == NULL || matrix2 == NULL || product == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix1->columns != matrix2->rows) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err = matrix_resize(product, matrix1->rows, matrix2->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < matrix1->rows; ++row) {
        for (matrix_size_t column = 0UL; column < matrix2->columns; ++column) {
            matrix_data_t sum = 0.0F;

            for (matrix_size_t common = 0UL; common < matrix1->columns;
                 ++common) {
                sum += MATRIX_INDEX(matrix1, row, common) *
                       MATRIX_INDEX(matrix2, common, column);
            }

            MATRIX_INDEX(product, row, column) = sum;
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_division(matrix_t const* matrix1,
                             matrix_t const* matrix2,
                             matrix_t* division)
{
    if (matrix1 == NULL || matrix2 == NULL || division == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_t matrix2_inverse;
    matrix_err_t err = matrix_inverse(matrix2, &matrix2_inverse);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_product(matrix1, &matrix2_inverse, division);
}

matrix_err_t matrix_power(matrix_t const* matrix,
                          matrix_size_t exponent,
                          matrix_t* power)
{
    if (matrix == NULL || power == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err =
        matrix_create_with_zeros(power, matrix->rows, matrix->columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t index = 0UL; index < matrix->rows; ++index) {
        MATRIX_INDEX(power, index, index) = 1.0F;
    }

    if (exponent == 0U) {
        return MATRIX_ERR_OK;
    }

    matrix_t temp;
    for (matrix_size_t index = 0UL; index < exponent - 1UL; ++index) {
        err = matrix_product(matrix, power, &temp);
        if (err != MATRIX_ERR_OK) {
            return err;
        }

        *power = temp;
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_trace(matrix_t const* matrix, matrix_data_t* trace)
{
    if (matrix == NULL || trace == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->rows != matrix->columns) {
        return MATRIX_ERR_DIMENSION;
    }

    *trace = 0.0F;
    for (matrix_size_t index = 0UL; index < matrix->rows; ++index) {
        *trace += MATRIX_INDEX(matrix, index, index);
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_rank(matrix_t const* matrix, matrix_size_t* rank)
{
    if (matrix == NULL || rank == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_t row_echelon_form;
    matrix_err_t err = matrix_initialize(&row_echelon_form, &matrix->allocator);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_row_echelon_form(matrix, &row_echelon_form);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    *rank = 0UL;
    for (matrix_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix_size_t column = 0UL; column < 3UL; ++column) {
            if (fabsf(MATRIX_INDEX(&row_echelon_form, row, column)) > 1E-6F) {
                (*rank)++;
                break;
            }
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_eigvals(matrix_t const* matrix,
                            matrix_data_t** eigvals,
                            matrix_size_t* eigvals_num)
{
    if (matrix == NULL || eigvals == NULL || eigvals_num == NULL) {
        return MATRIX_ERR_NULL;
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_print(matrix_t const* matrix, char const* endline)
{
    if (matrix == NULL || endline == NULL) {
        return MATRIX_ERR_NULL;
    }

    for (matrix_size_t row = 0UL; row < matrix->rows; ++row) {
        printf("[ ");

        for (matrix_size_t column = 0UL; column < matrix->columns; ++column) {
            printf("%f ", MATRIX_INDEX(matrix, row, column));
        }

        printf("]%s", endline);
    }

    return MATRIX_ERR_OK;
}
