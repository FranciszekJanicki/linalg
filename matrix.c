#include "matrix.h"
#include <string.h>

static void* matrix_allocate(matrix_t const* matrix, size_t size)
{
    if (matrix->interface.allocate == NULL) {
        return NULL;
    }

    return matrix->interface.allocate(size);
}

static void matrix_deallocate(matrix_t const* matrix, void* data)
{
    if (matrix->interface.deallocate == NULL) {
        return;
    }

    matrix->interface.deallocate(data);
}

static void matrix_vprint(matrix_t const* matrix, char const* format, ...)
{
    if (matrix->interface.vprint == NULL) {
        return;
    }

    va_list args;
    va_start(args, format);
    matrix->interface.vprint(format, args);
    va_end(args);
}

matrix_err_t matrix_initialize(matrix_t* matrix,
                               matrix_interface_t const* interface)
{
    if (matrix == NULL || interface == NULL) {
        return MATRIX_ERR_NULL;
    }

    memset(matrix, 0, sizeof(*matrix));
    memcpy(&matrix->interface, interface, sizeof(*interface));

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

matrix_size_t matrix_rows(matrix_t const* matrix)
{
    if (matrix == NULL) {
        return 0UL;
    }

    return matrix->rows;
}

matrix_size_t matrix_columns(matrix_t const* matrix)
{
    if (matrix == NULL) {
        return 0UL;
    }

    return matrix->columns;
}

matrix_err_t matrix_create(matrix_t* matrix,
                           matrix_size_t rows,
                           matrix_size_t columns)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_elem_t* data =
        matrix_allocate(matrix, sizeof(matrix_elem_t) * rows * columns);
    if (data == NULL) {
        return MATRIX_ERR_ALLOC;
    }

    matrix->data = data;
    matrix->rows = rows;
    matrix->columns = columns;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_create_eye(matrix_t* matrix, matrix_size_t dimensions)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_create(matrix, dimensions, dimensions);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_eye(matrix);
}

matrix_err_t matrix_create_with_elem(matrix_t* matrix,
                                     matrix_size_t rows,
                                     matrix_size_t columns,
                                     matrix_elem_t elem)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_create(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_with_elem(matrix, elem);
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

matrix_err_t matrix_create_from_array(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns,
                                      matrix_elem_t (*array)[rows][columns])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_create(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_from_array(matrix, array);
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

    matrix_elem_t* data =
        matrix_allocate(matrix, sizeof(matrix_elem_t) * rows * columns);
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

matrix_err_t matrix_resize_eye(matrix_t* matrix, matrix_size_t dimensions)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_resize(matrix, dimensions, dimensions);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_eye(matrix);
}

matrix_err_t matrix_resize_with_elem(matrix_t* matrix,
                                     matrix_size_t rows,
                                     matrix_size_t columns,
                                     matrix_elem_t elem)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_resize(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_with_elem(matrix, elem);
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

matrix_err_t matrix_resize_from_array(matrix_t* matrix,
                                      matrix_size_t rows,
                                      matrix_size_t columns,
                                      matrix_elem_t (*array)[rows][columns])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_resize(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_fill_from_array(matrix, array);
}

matrix_err_t matrix_fill_eye(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns) {
        return MATRIX_ERR_DIMENSION;
    }

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
            if (row == column) {
                MATRIX_INDEX(matrix, row, column) = 1.0F;
            } else {
                MATRIX_INDEX(matrix, row, column) = 0.0F;
            }
        }
    }
}

matrix_err_t matrix_fill_with_elem(matrix_t* matrix, matrix_elem_t elem)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    for (matrix_size_t index = 0UL; index < rows * columns; ++index) {
        matrix->data[index] = elem;
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_fill_with_zeros(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    memset(matrix->data, 0, sizeof(matrix_elem_t) * rows * columns);

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_fill_from_array(
    matrix_t* matrix,
    matrix_elem_t (*array)[matrix->rows][matrix->columns])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    memcpy(matrix->data, array, sizeof(matrix_elem_t) * rows * columns);

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_copy(matrix_t const* source, matrix_t* destination)
{
    if (source == NULL || destination == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = source->rows;
    matrix_size_t columns = source->columns;

    matrix_err_t err = matrix_resize(destination, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    memcpy(destination->data,
           source->data,
           sizeof(*source->data) * rows * columns);

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

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns || minor_row >= rows || minor_column >= columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err = matrix_resize(minor, rows - 1UL, columns - 1UL);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    matrix_size_t cof_row = 0UL;

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        if (row == minor_row) {
            continue;
        }

        matrix_size_t cof_column = 0UL;

        for (matrix_size_t column = 0UL; column < columns; ++column) {
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

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err = matrix_resize(complement, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    matrix_t minor;
    err = matrix_initialize(&minor, &complement->interface);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
            err = matrix_minor(matrix, row, column, &minor);
            if (err != MATRIX_ERR_OK) {
                matrix_delete(&minor);
                return err;
            }

            matrix_elem_t minor_determinant;
            err = matrix_determinant(&minor, &minor_determinant);
            if (err != MATRIX_ERR_OK) {
                matrix_delete(&minor);
                return err;
            }

            if ((row + column) & 1UL) {
                MATRIX_INDEX(complement, row, column) = -minor_determinant;
            } else {
                MATRIX_INDEX(complement, row, column) = minor_determinant;
            }
        }
    }

    return matrix_delete(&minor);
}

matrix_err_t matrix_adjoint(matrix_t const* matrix, matrix_t* adjoint)
{
    if (matrix == NULL || adjoint == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_t complement;
    matrix_err_t err = matrix_initialize(&complement, &adjoint->interface);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_complement(matrix, &complement);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_transposition(&complement, adjoint);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_delete(&complement);
}

matrix_err_t matrix_transposition(matrix_t const* matrix,
                                  matrix_t* transposition)
{
    if (matrix == NULL || transposition == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    matrix_err_t err = matrix_resize(transposition, columns, rows);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
            MATRIX_INDEX(transposition, column, row) =
                MATRIX_INDEX(matrix, row, column);
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_determinant(matrix_t const* matrix,
                                matrix_elem_t* determinant)
{
    if (matrix == NULL || determinant == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns) {
        return MATRIX_ERR_DIMENSION;
    }

    if (rows == 1UL) {
        *determinant = MATRIX_INDEX(matrix, 0UL, 0UL);

        return MATRIX_ERR_OK;
    } else if (rows == 2UL) {
        *determinant =
            MATRIX_INDEX(matrix, 0UL, 0UL) * MATRIX_INDEX(matrix, 1UL, 1UL) -
            MATRIX_INDEX(matrix, 0UL, 1UL) * MATRIX_INDEX(matrix, 1UL, 0UL);

        return MATRIX_ERR_OK;
    }

    matrix_t minor;
    matrix_err_t err = matrix_initialize(&minor, &matrix->interface);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    *determinant = 0.0F;

    for (matrix_size_t column = 0UL; column < columns; ++column) {
        err = matrix_minor(matrix, 0UL, column, &minor);
        if (err != MATRIX_ERR_OK) {
            return err;
        }

        matrix_elem_t minor_determinant;
        err = matrix_determinant(&minor, &minor_determinant);
        if (err != MATRIX_ERR_OK) {
            return err;
        }

        if (column & 1U) {
            *determinant -=
                MATRIX_INDEX(matrix, 0UL, column) * minor_determinant;
        } else {
            *determinant +=
                MATRIX_INDEX(matrix, 0UL, column) * minor_determinant;
        }
    }

    return matrix_delete(&minor);
}

matrix_err_t matrix_inversion(matrix_t const* matrix, matrix_t* inversion)
{
    if (matrix == NULL || inversion == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_t adjoint;
    matrix_err_t err = matrix_initialize(&adjoint, &inversion->interface);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_adjoint(matrix, &adjoint);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    matrix_elem_t determinant;
    err = matrix_determinant(matrix, &determinant);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    if (fabsf(determinant) < 1E-6F) {
        return MATRIX_ERR_SINGULAR;
    }

    err = matrix_scale(&adjoint, 1.0F / determinant, inversion);
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

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_t lower_triangular;
    matrix_err_t err =
        matrix_initialize(&lower_triangular, &upper_triangular->interface);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_lower_triangular(matrix, &lower_triangular);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    err = matrix_transposition(&lower_triangular, upper_triangular);
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

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    if (rows != columns) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err = matrix_resize(lower_triangular, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column <= row; ++column) {
            matrix_elem_t sum = 0.0F;

            for (matrix_size_t lower = 0UL; lower < column; ++lower) {
                sum += MATRIX_INDEX(lower_triangular, row, lower) *
                       MATRIX_INDEX(lower_triangular, column, lower);
            }

            if (row == column) {
                matrix_elem_t val = MATRIX_INDEX(matrix, row, row) - sum;
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

        for (matrix_size_t column = row + 1; column < rows; ++column) {
            MATRIX_INDEX(lower_triangular, row, column) = 0.0F;
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

    matrix_size_t rows = matrix1->rows;
    matrix_size_t columns = matrix1->columns;

    matrix_err_t err = matrix_resize(sum, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
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

    matrix_size_t rows = matrix1->rows;
    matrix_size_t columns = matrix1->columns;

    matrix_err_t err = matrix_resize(difference, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
            MATRIX_INDEX(difference, row, column) =
                MATRIX_INDEX(matrix1, row, column) -
                MATRIX_INDEX(matrix2, row, column);
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_scale(matrix_t const* matrix,
                          matrix_elem_t scalar,
                          matrix_t* scale)
{
    if (matrix == NULL || scale == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_size_t rows = matrix->rows;
    matrix_size_t columns = matrix->columns;

    matrix_err_t err = matrix_resize(scale, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
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

    matrix_size_t rows1 = matrix1->rows;
    matrix_size_t columns1 = matrix1->columns;

    matrix_size_t rows2 = matrix2->rows;
    matrix_size_t columns2 = matrix2->columns;

    if (columns1 != rows2) {
        return MATRIX_ERR_DIMENSION;
    }

    matrix_err_t err = matrix_resize(product, rows1, columns2);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t row1 = 0UL; row1 < rows1; ++row1) {
        for (matrix_size_t column2 = 0UL; column2 < columns2; ++column2) {
            matrix_elem_t sum = 0.0F;

            for (matrix_size_t row2_column1 = 0UL; row2_column1 < columns1;
                 ++row2_column1) {
                sum += MATRIX_INDEX(matrix1, row1, row2_column1) *
                       MATRIX_INDEX(matrix2, row2_column1, column2);
            }

            MATRIX_INDEX(product, row1, column2) = sum;
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

    matrix_t matrix2_inversion;
    matrix_err_t err = matrix_inversion(matrix2, &matrix2_inversion);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    return matrix_product(matrix1, &matrix2_inversion, division);
}

matrix_err_t matrix_power(matrix_t const* matrix,
                          matrix_size_t exponent,
                          matrix_t* power)
{
    if (matrix == NULL || power == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_err_t err = matrix_copy(matrix, power);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    for (matrix_size_t i = 0UL; i < exponent - 1UL; ++i) {
        err = matrix_product(matrix, power, power);
        if (err != MATRIX_ERR_OK) {
            return err;
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_rank(matrix_t const* matrix, matrix_size_t* rank)
{
    if (matrix == NULL || rank == NULL) {
        return MATRIX_ERR_NULL;
    }

    *rank = 0UL;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_eigvals(matrix_t const* matrix,
                            matrix_elem_t** eigvals,
                            matrix_size_t* eigvals_num)
{
    if (matrix == NULL || eigvals == NULL || eigvals_num == NULL) {
        return MATRIX_ERR_NULL;
    }

    *eigvals = NULL;
    *eigvals_num = 0UL;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_print(matrix_t const* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    for (matrix_size_t row = 0UL; row < matrix->rows; ++row) {
        for (matrix_size_t column = 0UL; column < matrix->columns; ++column) {
            matrix_vprint(matrix, "%f ", MATRIX_INDEX(matrix, row, column));
        }

        matrix_vprint(matrix, "\n");
    }

    matrix_vprint(matrix, "\n");

    return MATRIX_ERR_OK;
}