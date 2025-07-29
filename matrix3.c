#include "matrix3.h"
#include <stdio.h>
#include <string.h>

matrix3_err_t matrix3_fill_with_zeros(matrix3_t* matrix)
{
    if (matrix == NULL) {
        return MATRIX3_ERR_NULL;
    }

    memset(matrix->data, 0, sizeof(matrix->data));

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_fill_with_array(matrix3_t* matrix,
                                      matrix3_data_t (*array)[3U][3U])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            matrix->data[row][column] = (*array)[row][column];
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_sum(matrix3_t const* matrix1,
                          matrix3_t const* matrix2,
                          matrix3_t* sum)
{
    if (matrix1 == NULL || matrix2 == NULL || sum == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            sum->data[row][column] =
                matrix1->data[row][column] + matrix2->data[row][column];
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_difference(matrix3_t const* matrix1,
                                 matrix3_t const* matrix2,
                                 matrix3_t* difference)
{
    if (matrix1 == NULL || matrix2 == NULL || difference == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            difference->data[row][column] =
                matrix1->data[row][column] - matrix2->data[row][column];
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_scale(matrix3_t const* matrix,
                            matrix3_data_t scalar,
                            matrix3_t* scale)
{
    if (matrix == NULL || scale == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            scale->data[row][column] = matrix->data[row][column] * scalar;
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_product(matrix3_t const* matrix1,
                              matrix3_t const* matrix2,
                              matrix3_t* product)
{
    if (matrix1 == NULL || matrix2 == NULL || product == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            product->data[row][column] = 0.0f;

            for (matrix3_size_t k = 0U; k < 3U; ++k) {
                product->data[row][column] +=
                    matrix1->data[row][k] * matrix2->data[k][column];
            }
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_transpose(matrix3_t const* matrix, matrix3_t* transpose)
{
    if (matrix == NULL || transpose == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            transpose->data[column][row] = matrix->data[row][column];
        }
    }

    return MATRIX3_ERR_OK;
}

static inline matrix3_data_t matrix3_det_2x2(matrix3_data_t a,
                                             matrix3_data_t b,
                                             matrix3_data_t c,
                                             matrix3_data_t d)
{
    return a * d - b * c;
}

matrix3_err_t matrix3_det(matrix3_t const* matrix, matrix3_data_t* det)
{
    if (matrix == NULL || det == NULL) {
        return MATRIX3_ERR_NULL;
    }

    matrix3_data_t a = matrix->data[0U][0U];
    matrix3_data_t b = matrix->data[0U][1U];
    matrix3_data_t c = matrix->data[0U][2U];

    matrix3_data_t det_00 = matrix3_det_2x2(matrix->data[1U][1U],
                                            matrix->data[1U][2U],
                                            matrix->data[2U][1U],
                                            matrix->data[2U][2U]);

    matrix3_data_t det_01 = matrix3_det_2x2(matrix->data[1U][0U],
                                            matrix->data[1U][2U],
                                            matrix->data[2U][0U],
                                            matrix->data[2U][2U]);

    matrix3_data_t det_02 = matrix3_det_2x2(matrix->data[1U][0U],
                                            matrix->data[1U][1U],
                                            matrix->data[2U][0U],
                                            matrix->data[2U][1U]);

    *det = a * det_00 - b * det_01 + c * det_02;

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_inverse(matrix3_t const* matrix, matrix3_t* inverse)
{
    if (matrix == NULL || inverse == NULL) {
        return MATRIX3_ERR_NULL;
    }

    matrix3_data_t det;
    matrix3_err_t err = matrix3_det(matrix, &det);
    if (err != MATRIX3_ERR_OK) {
        return MATRIX3_ERR_FAIL;
    }

    if (det == 0.0F) {
        return MATRIX3_ERR_FAIL;
    }

    matrix3_t adjoint;
    err = matrix3_adjoint(matrix, &adjoint);
    if (err != MATRIX3_ERR_OK) {
        return MATRIX3_ERR_FAIL;
    }

    err = matrix3_scale(&adjoint, 1.0F / det, inverse);
    if (err != MATRIX3_ERR_OK) {
        return MATRIX3_ERR_FAIL;
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_minor(matrix3_t const* matrix,
                            matrix3_size_t minor_row,
                            matrix3_size_t minor_column,
                            matrix3_t* minor)
{
    if (matrix == NULL || minor == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U, cof_row = 0U; row < 3U; ++row) {
        if (row == minor_row) {
            continue;
        }

        for (matrix3_size_t column = 0U, cof_column = 0U; column < 3U;
             ++column) {
            if (column == minor_column) {
                continue;
            }

            minor->data[cof_row][cof_column] = matrix->data[row][column];

            ++cof_column;
        }

        ++cof_row;
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_complement(matrix3_t const* matrix, matrix3_t* complement)
{
    if (matrix == NULL || complement == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            matrix3_t minor;

            matrix3_err_t minor_result =
                matrix3_minor(matrix, row, column, &minor);

            if (minor_result != MATRIX3_ERR_OK) {
                return MATRIX3_ERR_FAIL;
            }

            matrix3_data_t det_minor = minor.data[0U][0U] * minor.data[1U][1U] -
                                       minor.data[0U][1U] * minor.data[1U][0U];

            complement->data[row][column] =
                (((row + column) & 1U) ? -1.0F : 1.0F) * det_minor;
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_adjoint(matrix3_t const* matrix, matrix3_t* adjoint)
{
    if (matrix == NULL || adjoint == NULL) {
        return MATRIX3_ERR_NULL;
    }

    matrix3_t complement;
    matrix3_err_t err = matrix3_complement(matrix, &complement);
    if (err != MATRIX3_ERR_OK) {
        return MATRIX3_ERR_FAIL;
    }

    err = matrix3_transpose(&complement, adjoint);
    if (err != MATRIX3_ERR_OK) {
        return MATRIX3_ERR_FAIL;
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_print(matrix3_t const* matrix)
{
    if (matrix == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0U; row < 3U; ++row) {
        printf("[ ");

        for (matrix3_size_t column = 0U; column < 3U; ++column) {
            printf("%f ", matrix->data[row][column]);
        }

        printf("]\n");
    }

    return MATRIX3_ERR_OK;
}
