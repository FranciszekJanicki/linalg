#include "matrix.h"
#include <string.h>

static matrix_elem_t* matrix_allocate(matrix_t const* matrix,
                                      matrix_size_t size)
{
    if (matrix->allocate == NULL) {
        return NULL;
    }

    return matrix->allocate(size);
}

static void matrix_deallocate(matrix_t const* matrix, matrix_elem_t* data)
{
    if (matrix->deallocate == NULL) {
        return;
    }

    matrix->deallocate(data);
}

matrix_err_t matrix_initialize(matrix_t* matrix,
                               matrix_allocate_t allocate,
                               matrix_deallocate_t deallocate)
{
    if (matrix == NULL || allocate == NULL || deallocate == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix->data = NULL;
    matrix->rows = 0UL;
    matrix->columns = 0UL;
    matrix->allocate = allocate;
    matrix->deallocate = deallocate;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_deinitialize(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix->data = NULL;
    matrix->rows = 0UL;
    matrix->columns = 0UL;
    matrix->allocate = NULL;
    matrix->deallocate = NULL;

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_create(matrix_t* matrix,
                           matrix_size_t rows,
                           matrix_size_t columns)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    matrix_elem_t* data = matrix_allocate(matrix, rows * columns);
    if (data == NULL) {
        return MATRIX_ERR_ALLOC;
    }

    matrix->data = data;
    matrix->rows = rows;
    matrix->columns = columns;

    return MATRIX_ERR_OK;
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

    return matrix_fill_from_array(matrix, rows, columns, array);
}

matrix_err_t matrix_delete(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (matrix->data) {
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

    if (matrix->rows == rows && matrix->columns == columns) {
        return MATRIX_ERR_OK;
    }

    if (matrix->rows == columns && matrix->columns == rows) {
        matrix->rows = rows;
        matrix->columns = columns;

        return MATRIX_ERR_OK;
    }

    matrix_elem_t* old_data = matrix->data;

    matrix_err_t err = matrix_create(matrix, rows, columns);
    if (err != MATRIX_ERR_OK) {
        return err;
    }

    if (old_data) {
        matrix_deallocate(matrix, old_data);
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_clear(matrix_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX_ERR_NULL;
    }

    return matrix_resize(matrix, 0UL, 0UL);
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
           sizeof(matrix_elem_t) * source->rows * source->columns);

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_fill_from_array(matrix_t* matrix,
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

    memcpy(matrix->data, array, sizeof(matrix_elem_t) * rows * columns);

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_move(matrix_t* source, matrix_t* destination)
{
    if (source == NULL || destination == NULL) {
        return MATRIX_ERR_NULL;
    }

    destination->data = source->data;
    destination->rows = source->rows;
    destination->columns = source->columns;

    source->data = NULL;
    source->rows = 0UL;
    source->columns = 0UL;

    return MATRIX_ERR_OK;
}

matrix_size_t matrix_rows(matrix_t const* matrix)
{
    return matrix->rows;
}

matrix_size_t matrix_columns(matrix_t const* matrix)
{
    return matrix->columns;
}

matrix_err_t matrix_minor(matrix_t const* matrix,
                          size_t row,
                          size_t column,
                          matrix_t* minor)
{}

matrix_err_t matrix_complement(matrix_t const* matrix, matrix_t* complement)
{}

matrix_err_t matrix_adjoint(matrix_t const* matrix, matrix_t* adjoint)
{}

matrix_err_t matrix_transposition(matrix_t const* matrix,
                                  matrix_t* transposition)
{}

matrix_err_t matrix_det(matrix_t const* matrix, matrix_elem_t* det)
{}

matrix_err_t matrix_inversion(matrix_t const* matrix, matrix_t* inversion)
{}

matrix_err_t matrix_upper_triangular(matrix_t const* matrix,
                                     matrix_t* upper_triangular)
{}

matrix_err_t matrix_lower_triangular(matrix_t const* matrix,
                                     matrix_t* lower_triangular)
{}

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

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
            matrix_index(sum, row, column) =
                matrix_index(matrix1, row, column) +
                matrix_index(matrix2, row, column);
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

    for (matrix_size_t row = 0UL; row < rows; ++row) {
        for (matrix_size_t column = 0UL; column < columns; ++column) {
            matrix_index(difference, row, column) =
                matrix_index(matrix1, row, column) -
                matrix_index(matrix2, row, column);
        }
    }

    return MATRIX_ERR_OK;
}

matrix_err_t matrix_scale(matrix_t const* matrix,
                          matrix_elem_t scalar,
                          matrix_t* scale)
{}

matrix_err_t matrix_product(matrix_t const* matrix1,
                            matrix_t const* matrix2,
                            matrix_t* product)
{}

matrix_err_t matrix_power(matrix_t const* matrix,
                          matrix_elem_t exponent,
                          matrix_t* power)
{}

matrix_err_t matrix_rank(matrix_err_t const* matrix, matrix_size_t* rank)
{}

matrix_err_t matrix_eigvals(matrix_t const* matrix,
                            matrix_elem_t** eigvals,
                            matrix_size_t* eigvals_num)
{}
