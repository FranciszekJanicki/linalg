#ifndef STACK_MATRIX_HPP
#define STACK_MATRIX_HPP

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <exception>
#include <print>
#include <stdexcept>
#include <utility>

namespace Linalg::Stack {

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    struct Matrix {
        using Data = std::array<std::array<Value, COLS>, ROWS>;
        using Init = std::initializer_list<std::initializer_list<Value>>;
        using Row = std::array<Value, COLS>;
        using Column = std::array<Value, ROWS>;

        [[nodiscard]] static auto make_data(Init const init) noexcept -> Data
        {
            Data result;
            for (std::size_t i{}; i < ROWS; ++i) {
                for (std::size_t j{}; j < COLS; ++j) {
                    result[i][j] = *((init.begin() + i)->begin() + j);
                }
            }
            return result;
        }

        Matrix() noexcept = default;

        Matrix(Init const init) noexcept : data{make_data(init)}
        {}

        Matrix(Matrix&& other) noexcept = default;
        Matrix(Matrix const& other) noexcept = default;

        [[nodiscard]] auto operator=(Matrix&& other) noexcept -> Matrix& = default;
        [[nodiscard]] auto operator=(Matrix const& other) noexcept -> Matrix& = default;

        ~Matrix() noexcept = default;

        [[nodiscard]] auto operator[](this Matrix const& self, std::size_t const row) -> Row const&
        {
            if (row > ROWS) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[row];
        }

        [[nodiscard]] auto operator[](this Matrix& self, std::size_t const row) -> Row&
        {
            if (row > ROWS) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[row];
        }

        [[nodiscard]] auto operator[](this Matrix const& self, std::size_t const row, std::size_t const col)
            -> Value const&
        {
            if (row > ROWS || col > COLS) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[row][col];
        }

        [[nodiscard]] auto operator[](this Matrix& self, std::size_t const row, std::size_t const col) -> Value&
        {
            if (row > ROWS || col > COLS) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[row][col];
        }

        [[nodiscard]] auto operator<=>(this Matrix const& self, Matrix const& other) noexcept -> bool = default;

        [[nodiscard]] auto operator=(this Matrix& self, Init const init) -> Matrix
        {
            self.data = make_data(init);
            return self;
        }

        auto print(this Matrix const& self) noexcept -> void
        {
            std::print("[");
            if (!self.data.empty()) {
                for (auto const& row : self.data) {
                    std::print("[");
                    if (!row.empty()) {
                        for (auto const col : row) {
                            std::print("{}", col);
                            if (col != row.back()) {
                                std::print(", ");
                            }
                        }
                    }
                    std::print("]");
                    if (row != self.data.back()) {
                        std::print(",\n");
                    }
                }
            }
            std::print("]\n");
        }

        [[nodiscard]] auto is_square(this Matrix const& self) noexcept -> bool
        {
            return ROWS == COLS;
        }

        [[nodiscard]] auto rows(this Matrix const& self) noexcept -> std::size_t
        {
            return ROWS;
        }

        [[nodiscard]] auto cols(this Matrix const& self) noexcept -> std::size_t
        {
            return COLS;
        }

        [[nodiscard]] auto operator+=(this Matrix& self, Matrix const& other) noexcept -> Matrix
        {
            self = matrix_sum(self, other);
            return self;
        }

        [[nodiscard]] auto operator-=(this Matrix& self, Matrix const& other) noexcept -> Matrix
        {
            self = matrix_difference(self, other);
            return self;
        }

        [[nodiscard]] auto operator*=(this Matrix& self, Matrix const& other) -> Matrix
        {
            try {
                self = matrix_product(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] auto operator*=(this Matrix& self, Value const scale) -> Matrix
        {
            try {
                self = matrix_scale(self, scale);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] auto operator/=(this Matrix& self, Value const scale) -> Matrix
        {
            try {
                self = matrix_scale(self, 1 / scale);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] auto operator/=(this Matrix& self, Matrix const& other) -> Matrix
        {
            try {
                self = matrix_product(self, matrix_inverse(other));
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] auto operator^=(this Matrix& self, Value const power) -> Matrix
        {
            self = matrix_power(self, power);
            return self;
        }

        Data data{};
    };

    template <std::floating_point Value, std::size_t COLS>
    using Row = Matrix<Value, 1, COLS>;

    template <std::floating_point Value, std::size_t ROWS>
    using Column = Matrix<Value, ROWS, 1>;

    template <std::floating_point Value, std::size_t DIMS>
    using Square = Matrix<Value, DIMS, DIMS>;

    template <std::floating_point Value>
    using Scalar = Matrix<Value, 1, 1>;

    template <std::floating_point Value,
              std::size_t LEFT_ROWS,
              std::size_t LEFT_COLS_RIGHT_ROWS,
              std::size_t RIGHT_COLS>
    using Product = Matrix<Value, LEFT_ROWS, RIGHT_COLS>;

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    using Transpose = Matrix<Value, COLS, ROWS>;

    template <std::floating_point Value, std::size_t DIMS>
    using Minor = Square<Value, DIMS - 1>;

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto make_eye() noexcept -> Square<Value, DIMS>
    {
        Square<Value, DIMS> result;
        for (std::size_t i{}; i < DIMS; ++i) {
            for (std::size_t j{}; j < DIMS; ++j) {
                result[i, j] = (i == j) ? Value{1} : Value{0};
            }
        }
        return result;
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_minor(Square<Value, DIMS> const& matrix, std::size_t const row, std::size_t const column)
    {
        if (row >= DIMS || column >= DIMS) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }
        if constexpr (DIMS == 1) {
            return matrix;
        }

        if constexpr (DIMS > 1) {
            std::size_t cof_i{};
            std::size_t cof_j{};
            Square<Value, DIMS - 1> result{};
            for (std::size_t i{}; i < DIMS - 1; ++i) {
                for (std::size_t j{}; j < DIMS - 1; ++j) {
                    if (i != row && j != column) {
                        result[cof_i, cof_j++] = matrix[i, j];
                        if (cof_j == DIMS - 1) {
                            cof_j = 0;
                            ++cof_i;
                        }
                    }
                }
            }
            return result;
        }
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_complement(Square<Value, DIMS> const& matrix) -> Square<Value, DIMS>
    {
        if constexpr (DIMS == 1) {
            return matrix;
        }

        if constexpr (DIMS > 1) {
            Square<Value, DIMS> result{};
            for (std::size_t i{}; i < DIMS; ++i) {
                for (std::size_t j{}; j < DIMS; ++j) {
                    try {
                        result[i, j] = ((i + j) % 2 == 0 ? 1 : -1) * matrix_det(matrix_minor(matrix, i, j));
                    } catch (std::runtime_error const& error) {
                        throw error;
                    }
                }
            }
            return result;
        }
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_adjoint(Square<Value, DIMS> const& matrix) -> Square<Value, DIMS>
    {
        try {
            return matrix_transpose(matrix_complement(matrix));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto matrix_transpose(Matrix<Value, ROWS, COLS> const& matrix) noexcept -> Matrix<Value, COLS, ROWS>
    {
        Matrix<Value, COLS, ROWS> result{};
        for (std::size_t i{}; i < ROWS; ++i) {
            for (std::size_t j{}; j < COLS; ++j) {
                result[i, j] = matrix[j, i];
            }
        }
        return result;
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_det(Square<Value, DIMS> const& matrix) -> Value
    {
        if constexpr (DIMS == 1) {
            return matrix[0, 0];
        }

        if constexpr (DIMS == 2) {
            return (matrix[0, 0] * matrix[1, 1]) - (matrix[1, 0] * matrix[0, 1]);
        }

        if constexpr (DIMS > 2) {
            try {
                Value det{};
                for (std::size_t i{}; i < DIMS; ++i) {
                    det += (i % 2 == 0 ? 1 : -1) * matrix[0, i] * matrix_det(matrix_minor(matrix, 0, i));
                }
                return det;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_inverse(Square<Value, DIMS> const& matrix) -> Square<Value, DIMS>
    {
        try {
            return matrix_scale(matrix_adjoint(matrix), 1 / matrix_det(matrix));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_upper_triangular(Square<Value, DIMS> const& matrix) -> Square<Value, DIMS>
    {
        try {
            return matrix_transpose(matrix_lower_triangular(matrix));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_lower_triangular(Square<Value, DIMS> const& matrix) -> Square<Value, DIMS>
    {
        if constexpr (DIMS == 1) {
            return matrix;
        }

        if constexpr (DIMS > 1) {
            Square<Value, DIMS> result;
            for (std::size_t i{}; i < DIMS; ++i) {
                for (std::size_t j{}; j <= i; ++j) {
                    Value sum{};
                    for (std::size_t k{}; k < j; ++k) {
                        if (j == i) {
                            sum += std::pow(result[j, k], 2);
                            result[j, j] = std::sqrt(matrix[j, j] - sum);
                        } else {
                            sum += (result[i, k] * result[j, k]);
                            result[i, j] = (matrix[i, j] - sum) / result[j, j];
                        }
                    }
                }
            }
            return result;
        }
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto matrix_sum(Matrix<Value, ROWS, COLS> const& left,
                                  Matrix<Value, ROWS, COLS> const& right) noexcept -> Matrix<Value, ROWS, COLS>
    {
        Matrix<Value, ROWS, COLS> result;
        for (std::size_t i{}; i < ROWS; ++i) {
            for (std::size_t j{}; j < COLS; ++j) {
                result[i, j] = left[i, j] + right[i, j];
            }
        }
        return result;
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto matrix_difference(Matrix<Value, ROWS, COLS> const& left,
                                         Matrix<Value, ROWS, COLS> const& right) noexcept -> Matrix<Value, ROWS, COLS>
    {
        Matrix<Value, ROWS, COLS> result;
        for (std::size_t i{}; i < ROWS; ++i) {
            for (std::size_t j{}; j < COLS; ++j) {
                result[i, j] = left[i, j] - right[i, j];
            }
        }
        return result;
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto matrix_scale(Matrix<Value, ROWS, COLS> const& matrix, Value const scale)
        -> Matrix<Value, ROWS, COLS>
    {
        if (scale == std::numeric_limits<Value>::max()) {
            throw std::runtime_error{"Multiplication by inf!\n"};
        }

        Matrix<Value, ROWS, COLS> result;
        for (std::size_t i{}; i < ROWS; ++i) {
            for (std::size_t j{}; j < COLS; ++j) {
                result[i, j] = matrix[i, j] * scale;
            }
        }
        return result;
    }

    template <std::floating_point Value,
              std::size_t LEFT_ROWS,
              std::size_t LEFT_COLS_RIGHT_ROWS,
              std::size_t RIGHT_COLS>
    [[nodiscard]] auto matrix_product(Matrix<Value, LEFT_ROWS, LEFT_COLS_RIGHT_ROWS> const& left,
                                      Matrix<Value, LEFT_COLS_RIGHT_ROWS, RIGHT_COLS> const& right)
        -> Matrix<Value, LEFT_ROWS, RIGHT_COLS>
    {
        Matrix<Value, LEFT_ROWS, RIGHT_COLS> result;
        for (std::size_t i{}; i < LEFT_ROWS; ++i) {
            for (std::size_t j{}; j < RIGHT_COLS; ++j) {
                Value sum{};
                for (std::size_t k{}; k < LEFT_COLS_RIGHT_ROWS; ++k) {
                    sum += left[i, k] * right[k, j];
                    if (sum >= std::numeric_limits<Value>::max()) {
                        throw std::runtime_error{"Multiplication by inf"};
                    }
                }
                result[i, j] = sum;
            }
        }
        return result;
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_power(Square<Value, DIMS> const& matrix, Value const power) noexcept
        -> Square<Value, DIMS>
    {
        if (power == 1) {
            return matrix;
        }

        Square<Value, DIMS> result{matrix};
        for (std::size_t i{}; i < power - 1; ++i) {
            result = matrix_product(result, matrix);
        }
        return result;
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto matrix_rank(Matrix<Value, ROWS, COLS> const& matrix) noexcept -> std::size_t
    {
        if constexpr (ROWS == COLS) {
            if (auto det{matrix_det(matrix)}; std::abs(det) < 0.00001F) {
                return ROWS - 1UL;
            }
            return ROWS;
        }
        if constexpr (ROWS > COLS) {
            return COLS;
        }
        if constexpr (ROWS < COLS) {
            return ROWS;
        }
    }

    template <std::floating_point Value, std::size_t ELEMS>
    using Eigvals = std::array<Value, ELEMS>;

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto matrix_eigvals(Square<Value, DIMS> const& matrix) noexcept -> Eigvals<Value, DIMS>
    {}

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto operator+(Matrix<Value, ROWS, COLS> const& left, Matrix<Value, ROWS, COLS> const& right) noexcept
        -> Matrix<Value, ROWS, COLS>
    {
        return matrix_sum(left, right);
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto operator-(Matrix<Value, ROWS, COLS> const& left, Matrix<Value, ROWS, COLS> const& right) noexcept
        -> Matrix<Value, ROWS, COLS>
    {
        return matrix_difference(left, right);
    }

    template <std::floating_point Value,
              std::size_t LEFT_ROWS,
              std::size_t LEFT_COLS_RIGHT_ROWS,
              std::size_t RIGHT_COLS>
    [[nodiscard]] auto operator*(Matrix<Value, LEFT_ROWS, LEFT_COLS_RIGHT_ROWS> const& left,
                                 Matrix<Value, LEFT_COLS_RIGHT_ROWS, RIGHT_COLS> const& right) noexcept
        -> Matrix<Value, LEFT_ROWS, RIGHT_COLS>
    {
        return matrix_product(left, right);
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto operator*(Value const scale, Matrix<Value, ROWS, COLS> const& matrix)
        -> Matrix<Value, ROWS, COLS>
    {
        try {
            return matrix_scale(matrix, scale);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto operator*(Matrix<Value, ROWS, COLS> const& matrix, Value const scale)
        -> Matrix<Value, ROWS, COLS>
    {
        try {
            return matrix_scale(matrix, scale);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <std::floating_point Value, std::size_t ROWS, std::size_t COLS>
    [[nodiscard]] auto operator/(Matrix<Value, ROWS, COLS> const& matrix, Value const scale)
        -> Matrix<Value, ROWS, COLS>
    {
        try {
            return matrix_scale(matrix, 1 / scale);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto operator/(Square<Value, DIMS> const& left, Square<Value, DIMS> const& right)
        -> Square<Value, DIMS>
    {
        try {
            return matrix_product(left, matrix_inverse(right));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <std::floating_point Value, std::size_t DIMS>
    [[nodiscard]] auto operator^(Square<Value, DIMS> const& matrix, Value const power) -> Square<Value, DIMS>
    {
        return matrix_power(matrix, power);
    }

}; // namespace Linalg::Stack

#endif // STACK_VECTOR_HPP