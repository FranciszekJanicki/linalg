#include "matrix3.h"
#include <math.h>
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
                                      const matrix3_data_t (*array)[3U][3U])
{
    if (matrix == NULL || array == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
            matrix->data[row][column] = (*array)[row][column];
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_transpose(matrix3_t const* matrix, matrix3_t* transpose)
{
    if (matrix == NULL || transpose == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
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

    if (fabsf(det) < 1E-6F) {
        return MATRIX3_ERR_SINGULAR;
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

    for (matrix3_size_t row = 0U, cof_row = 0UL; row < 3UL; ++row) {
        if (row == minor_row) {
            continue;
        }

        for (matrix3_size_t column = 0U, cof_column = 0UL; column < 3UL;
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

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
            matrix3_t minor;

            matrix3_err_t minor_result =
                matrix3_minor(matrix, row, column, &minor);

            if (minor_result != MATRIX3_ERR_OK) {
                return MATRIX3_ERR_FAIL;
            }

            matrix3_data_t minor_det = minor.data[0U][0U] * minor.data[1U][1U] -
                                       minor.data[0U][1U] * minor.data[1U][0U];

            complement->data[row][column] =
                (((row + column) & 1U) ? -1.0F : 1.0F) * minor_det;
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

matrix3_err_t matrix3_upper_triangular(matrix3_t const* matrix,
                                       matrix3_t* upper_triangular)
{
    if (matrix == NULL || upper_triangular == NULL) {
        return MATRIX3_ERR_NULL;
    }

    matrix3_t lower_triangular;
    matrix3_err_t err = matrix3_lower_triangular(matrix, &lower_triangular);
    if (err != MATRIX3_ERR_OK) {
        return err;
    }

    return matrix3_transpose(&lower_triangular, upper_triangular);
}

matrix3_err_t matrix3_lower_triangular(matrix3_t const* matrix,
                                       matrix3_t* lower_triangular)
{
    if (matrix == NULL || lower_triangular == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column <= row; ++column) {
            matrix3_data_t sum = 0.0F;

            for (matrix3_size_t lower = 0UL; lower < column; ++lower) {
                sum += lower_triangular->data[row][lower] *
                       lower_triangular->data[column][lower];
            }

            if (row == column) {
                matrix3_data_t val = matrix->data[row][row] - sum;
                if (val <= 0.0F) {
                    return MATRIX3_ERR_FAIL;
                }

                lower_triangular->data[row][column] = sqrtf(val);
            } else {
                lower_triangular->data[row][column] =
                    (1.0F / lower_triangular->data[column][column]) *
                    (matrix->data[row][column] - sum);
            }
        }

        for (matrix3_size_t column = row + 1UL; column < 3UL; ++column) {
            lower_triangular->data[row][column] = 0.0F;
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_row_echelon_form(matrix3_t const* matrix,
                                       matrix3_t* row_echelon_form)
{
    if (matrix == NULL || row_echelon_form == NULL) {
        return MATRIX3_ERR_NULL;
    }

    *row_echelon_form = *matrix;

    for (matrix3_size_t pivot = 0UL; pivot < 3UL; ++pivot) {
        matrix3_size_t max_row = pivot;
        for (matrix3_size_t row = pivot + 1UL; row < 3UL; ++row) {
            if (fabsf(row_echelon_form->data[row][pivot]) >
                fabsf(row_echelon_form->data[max_row][pivot])) {
                max_row = row;
            }
        }

        if (fabsf(row_echelon_form->data[max_row][pivot]) < 1e-6F) {
            continue;
        }

        if (max_row != pivot) {
            for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
                matrix3_data_t temp = row_echelon_form->data[pivot][column];
                row_echelon_form->data[pivot][column] =
                    row_echelon_form->data[max_row][column];
                row_echelon_form->data[max_row][column] = temp;
            }
        }

        for (matrix3_size_t row = pivot + 1UL; row < 3UL; ++row) {
            matrix3_data_t factor = row_echelon_form->data[row][pivot] /
                                    row_echelon_form->data[pivot][pivot];
            for (matrix3_size_t col = pivot; col < 3UL; ++col) {
                row_echelon_form->data[row][col] -=
                    factor * row_echelon_form->data[pivot][col];
            }
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

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
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

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
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

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
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

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
            product->data[row][column] = 0.0f;

            for (matrix3_size_t common = 0UL; common < 3UL; ++common) {
                product->data[row][column] +=
                    matrix1->data[row][common] * matrix2->data[common][column];
            }
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_division(matrix3_t const* matrix1,
                               matrix3_t const* matrix2,
                               matrix3_t* division)
{
    if (matrix1 == NULL || matrix2 == NULL || division == NULL) {
        return MATRIX3_ERR_NULL;
    }

    matrix3_t matrix2_inverse;
    matrix3_err_t err = matrix3_inverse(matrix1, &matrix2_inverse);
    if (err != MATRIX3_ERR_OK) {
        return err;
    }

    return matrix3_product(matrix1, &matrix2_inverse, division);
}

matrix3_err_t matrix3_power(matrix3_t const* matrix,
                            matrix3_size_t exponent,
                            matrix3_t* power)
{
    if (matrix == NULL || power == NULL) {
        return MATRIX3_ERR_NULL;
    }

    matrix3_err_t err = matrix3_fill_with_zeros(power);
    if (err != MATRIX3_ERR_OK) {
        return err;
    }

    for (matrix3_size_t index = 0UL; index < 3UL; ++index) {
        power->data[index][index] = 1.0F;
    }

    if (exponent == 0U) {
        return MATRIX3_ERR_OK;
    }

    matrix3_t temp;
    for (matrix3_size_t index = 0UL; index < exponent; ++index) {
        err = matrix3_product(power, matrix, &temp);
        if (err != MATRIX3_ERR_OK) {
            return err;
        }

        *power = temp;
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_trace(matrix3_t const* matrix, matrix3_data_t* trace)
{
    if (matrix == NULL || trace == NULL) {
        return MATRIX3_ERR_NULL;
    }

    *trace = 0.0F;
    for (matrix3_size_t index = 0UL; index < 3UL; ++index) {
        *trace += matrix->data[index][index];
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_rank(matrix3_t const* matrix, matrix3_size_t* rank)
{
    if (matrix == NULL || rank == NULL) {
        return MATRIX3_ERR_NULL;
    }

    matrix3_t row_echelon_form;
    matrix3_err_t err = matrix3_row_echelon_form(matrix, &row_echelon_form);
    if (err != MATRIX3_ERR_OK) {
        return err;
    }

    *rank = 0UL;
    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
            if (fabsf(row_echelon_form.data[row][column]) > 1E-6F) {
                (*rank)++;
                break;
            }
        }
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_eigvals(matrix3_t const* matrix,
                              matrix3_data_t (*eigvals)[3U])
{
    if (matrix == NULL || eigvals == NULL) {
        return MATRIX3_ERR_NULL;
    }

    return MATRIX3_ERR_OK;
}

matrix3_err_t matrix3_print(matrix3_t const* matrix, matrix3_print_t print, char const* endline)
{
    if (matrix == NULL || print == NULL || endline == NULL) {
        return MATRIX3_ERR_NULL;
    }

    for (matrix3_size_t row = 0UL; row < 3UL; ++row) {
        print("[ ");

        for (matrix3_size_t column = 0UL; column < 3UL; ++column) {
            print("%f ", matrix->data[row][column]);
        }

        print("]%endline");
    }

    print("\n");

    return MATRIX3_ERR_OK;
}
