#include "matrix.h"

matrix_err_t matrix_initialize(matrix_t* matrix,
                               matrix_size_t rows,
                               matrix_size_t columns,
                               matrix_interface_t const* interfacem)
{}

matrix_err_t matrix_deinitialize(matrix_t* matrix)
{}

matrix_err_t matrix_print(matrix_t const* matrix)
{}

matrix_size_t matrix_rows(matrix_t const* matrix)
{}

matrix_size_t matrix_columns(matrix_t const* matrix)
{}

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
{}

matrix_err_t matrix_difference(matrix_t const* matrix1,
                               matrix_t const* matrix2,
                               matrix_t* difference)
{}

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
