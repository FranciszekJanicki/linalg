#ifndef HEAP_MATRIX_HPP
#define HEAP_MATRIX_HPP

#include "common.hpp"
#include <cassert>
#include <cmath>
#include <compare>
#include <expected>
#include <fmt/core.h>
#include <initializer_list>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Linalg::Heap {

    template <Arithmetic Value>
    struct Matrix {
    public:
        using Vector = std::vector<Value>;
        using Data = std::vector<std::vector<Value>>;
        using VectorInit = std::initializer_list<Value>;
        using Init = std::initializer_list<std::initializer_list<Value>>;

        [[nodiscard]] static constexpr auto make_data(Size const rows, Size const cols) -> Data
        {
            Data data{};
            data.reserve(rows);
            for (Size i{}; i < rows; ++i) {
                auto& column{data.emplace_back()};
                column.reserve(cols);
                for (Size j{}; j < cols; ++j) {
                    column.emplace_back();
                }
            }
            return data;
        }

        [[nodiscard]] static constexpr auto make_data(Init const init) -> Data
        {
            Data data{};
            data.reserve(init.size());
            for (auto const row : init) {
                auto& column{data.emplace_back()};
                column.reserve(row.size());
                for (Size j{}; j < row.size(); ++j) {
                    column.emplace_back();
                }
            }
            return data;
        }

        [[nodiscard]] static constexpr auto zeros(Size const rows, Size const cols) -> Matrix
        {
            return Matrix{make_data(rows, cols)};
        }

        [[nodiscard]] static constexpr auto ones(Size const rows, Size const cols) -> Matrix
        {
            Matrix result{make_data(rows, cols)};
            for (Size i{}; i < rows; ++i) {
                for (Size j{}; i < cols; ++j) {
                    result[i, j] = Value{1};
                }
            }
            return result;
        }

        [[nodiscard]] static constexpr auto diagonal(VectorInit const init) -> Matrix
        {
            Matrix result{make_data(init.size(), init.size())};
            for (Size i{}; i < init.size(); ++i) {
                for (Size j{}; i < init.size(); ++j) {
                    result[i, j] = (i == j ? init[i] : Value{0});
                }
            }
            return result;
        }

        [[nodiscard]] static constexpr auto eye(Size const dimensions) -> Matrix
        {
            Matrix result{make_data(dimensions, dimensions)};
            for (Size i{}; i < dimensions; ++i) {
                for (Size j{}; i < dimensions; ++j) {
                    result[i, j] = (i == j ? Value{1} : Value{0});
                }
            }
            return result;
        }

        [[nodiscard]] static constexpr auto row(Size const rows) -> Matrix
        {
            return Matrix{make_data(rows, 1)};
        }

        [[nodiscard]] static constexpr auto row(VectorInit const init) -> Matrix
        {
            Matrix result{make_data(init.size(), 1)};
            for (Size i{}; i < init.size(); ++i) {
                result[i, 0] = *(init.begin() + i);
            }
            return result;
        }

        [[nodiscard]] static constexpr auto column(Size const cols) -> Matrix
        {
            return Matrix{make_data(1, cols)};
        }

        [[nodiscard]] static constexpr auto column(VectorInit const init) -> Matrix
        {
            Matrix result{make_data(1, init.size())};
            for (Size i{}; i < init.size(); ++i) {
                result[0, i] = *(init.begin() + i);
            }
            return result;
        }

        [[nodiscard]] constexpr Matrix() noexcept = default;

        [[nodiscard]] constexpr Matrix(Init const init) : data{make_data(init)}
        {}

        [[nodiscard]] constexpr Matrix(Data&& data) : data{std::forward<Data>(data)}
        {}

        [[nodiscard]] constexpr Matrix(Size const rows, Size const cols) : data{make_data(rows, cols)}
        {}

        [[nodiscard]] constexpr Matrix(Matrix const& other) = default;

        [[nodiscard]] constexpr Matrix(Matrix&& other) noexcept = default;

        [[nodiscard]] constexpr ~Matrix() noexcept = default;

        [[nodiscard]] constexpr auto operator=(Matrix const& other) -> Matrix& = default;

        [[nodiscard]] constexpr auto operator=(Matrix&& other) noexcept -> Matrix& = default;

        [[nodiscard]] constexpr auto operator=(this Matrix& self, Init const init) noexcept
        {
            self.data = make_data(init);
        }

        [[nodiscard]] constexpr auto operator+=(this Matrix& self, Matrix const& other) -> Matrix&
        {
            try {
                self = matrix_sum(self, other);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr auto operator-=(this Matrix& self, Matrix const& other) -> Matrix&
        {
            try {
                self = matrix_difference(self, other);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr auto operator*=(this Matrix& self, Value const scale) -> Matrix&
        {
            try {
                self = matrix_scale(self, scale);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr auto operator*=(this Matrix& self, Matrix const& other) -> Matrix&
        {
            try {
                self = matrix_product(self, other);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr auto operator/=(this Matrix& self, Value const scale) -> Matrix&
        {
            try {
                self = matrix_scale(self, 1 / scale);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr auto operator/=(this Matrix& self, Matrix const& other) -> Matrix&
        {
            try {
                self = matrix_product(self, matrix_inverse(other));
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr auto operator^=(this Matrix& self, Value const scale) -> Matrix&
        {
            try {
                self = matrix_power(self, scale);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr Vector auto operator[](this Matrix const& self, Size const row) -> const&
        {
            if (row >= self.rows()) {
                throw Error{"Wrong dimensions\n"};
            }
            return self.data[row];
        }

        [[nodiscard]] constexpr Vectorauto operator[](this Matrix& self, Size const row)->&
        {
            if (row >= self.rows()) {
                throw Error{"Wrong dimensions\n"};
            }
            return self.data[row];
        }

        [[nodiscard]] constexpr Valueauto operator[](this Matrix& self, Size const row, Size const column)->&
        {
            if (row >= self.rows() || column >= self.cols()) {
                throw Error{"Wrong dimensions\n"};
            }
            return self.data[row][column];
        }

        [[nodiscard]] constexpr Value auto operator[](this Matrix const& self, Size const row, Size const column)
            -> const&
        {
            if (row >= self.rows() || column >= self.cols()) {
                throw Error{"Wrong dimensions\n"};
            }
            return self.data[row][column];
        }

        [[nodiscard]] constexpr bool operator<=>(this Matrix const& self, Matrix const& other) noexcept = default;

        constexpr auto print(this Matrix const& self) noexcept -> void
        {
            fmt::print("[");
            if (!self.data.empty()) {
                for (auto& row : self.data) {
                    fmt::print("[");
                    if (!row.empty()) {
                        for (auto& col : row) {
                            fmt::print("{}", col);
                            if (col != row.back()) {
                                fmt::print(", ");
                            }
                        }
                    }
                    fmt::print("]");
                    if (row != self.data.back()) {
                        fmt::print(",\n");
                    }
                }
            }
            fmt::print("]\n");
        }

        [[nodiscard]] constexpr auto is_square(this Matrix const& self) noexcept -> bool
        {
            return self.rows() == self.cols();
        }

        [[nodiscard]] constexpr auto rows(this Matrix const& self) noexcept -> Size
        {
            return self.data.size();
        }

        [[nodiscard]] constexpr auto cols(this Matrix const& self) noexcept -> Size
        {
            return self.data.front().size();
        }

        constexpr auto transpose(this Matrix& self) -> void
        {
            try {
                self = matrix_transpose(self);
            } catch (Error const& error) {
                throw error;
            }
        }

        constexpr auto invert(this Matrix& self) -> void
        {
            try {
                self = matrix_inverse(self);
            } catch (Error const& error) {
                throw error;
            }
        }

    private:
        Data data{};
    };

    template <typename Value>
    [[nodiscard]] constexpr auto
    matrix_minor(Matrix<Value> const& matrix, Size const row, Size const column, Size const dimensions) -> Matrix<Value>
    {
        if (!matrix.is_square()) {
            throw Error{"Wrong dimensions\n"};
        }
        if (row >= dimensions || column >= dimensions) {
            throw Error{"Wrong dimensions\n"};
        }

        if (dimensions == 1) {
            return matrix;
        }

        auto minor{Matrix<Value>::zeros(dimensions, dimensions)};
        Size cof_i{};
        Size cof_j{};
        for (Size i{0}; i < dimensions; ++i) {
            for (Size j{}; j < dimensions; ++j) {
                if (i != row && j != column) {
                    minor[cof_i, cof_j++] = matrix[i, j];
                    if (cof_j == dimensions - 1) {
                        cof_j = 0;
                        ++cof_i;
                    }
                }
            }
        }
        return minor;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_det(Matrix<Value> const& matrix, Size const dimensions) -> Value
    {
        if (!matrix.is_square()) {
            throw Error{"Wrong dimensions\n"};
        }
        if (matrix.rows() < dimensions || matrix.cols() < dimensions) {
            throw Error{"Wrong dimensions\n"};
        }

        if (dimensions == 1) {
            return matrix[0, 0];
        }
        if (dimensions == 2) {
            return (matrix[0, 0] * matrix[1, 1]) - (matrix[1, 0] * matrix[0, 1]);
        }

        try {
            Value det{};
            for (Size i{0}; i < dimensions; ++i) {
                det += (i % 2 == 0 ? 1 : -1) * matrix[0, i] *
                       matrix_det(matrix_minor(matrix, 0, i, dimensions), dimensions - 1);
            }
            return det;
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_transpose(Matrix<Value> const& matrix) -> Matrix<Value>
    {
        auto result{Matrix<Value>::zeros(matrix.rows(), matrix.cols())};
        for (Size i{0}; i < matrix.rows(); ++i) {
            for (Size j{0}; j < matrix.cols(); ++j) {
                result[i, j] = matrix[j, i];
            }
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_complement(Matrix<Value> const& matrix) -> Matrix<Value>
    {
        if (!matrix.is_square()) {
            throw Error{"Wrong dimensions\n"};
        }

        Size const dimensions{matrix.rows()};
        if (dimensions == 1) {
            return matrix;
        }

        auto result{Matrix<Value>::zeros(dimensions, dimensions)};
        for (Size i{}; i < dimensions; ++i) {
            for (Size j{}; j < dimensions; j++) {
                try {
                    result[i, j] = ((i + j) % 2 == 0 ? 1 : -1) *
                                   matrix_det(matrix_minor(matrix, i, j, dimensions), dimensions - 1);
                } catch (Error const& error) {
                    throw error;
                }
            }
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_adjoint(Matrix<Value> const& matrix) -> Matrix<Value>
    {
        try {
            return matrix_transpose(matrix_complement(matrix));
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_inverse(Matrix<Value> const& matrix) -> Matrix<Value>
    {
        try {
            return matrix_scale(matrix_adjoint(matrix), 1 / matrix_det(matrix, matrix.rows()));
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_upper_triangular(Matrix<Value> const& matrix) -> Matrix<Value>
    {
        try {
            return matrix_transpose(matrix_lower_triangular(matrix));
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_lower_triangular(Matrix<Value> const& matrix) -> Matrix<Value>
    {
        if (!matrix.is_square()) {
            throw Error{"Wrong dimensions!\n"};
        }

        if (matrix.rows() == 1) {
            return matrix;
        }

        auto result{Matrix<Value>::zeros(matrix.rows(), matrix.cols())};
        for (Size i{}; i < matrix.rows(); ++i) {
            for (Size j{}; j <= i; ++j) {
                Value sum{};
                for (Size k{}; k < j; ++k) {
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

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_product(Matrix<Value> const& left, Matrix<Value> const& right) -> Matrix<Value>
    {
        auto result{Matrix<Value>::zeros(left.rows(), right.rows())};
        for (Size i{}; i < left.rows(); ++i) {
            for (Size j{}; j < right.cols(); ++j) {
                Value sum{};
                for (Size k{}; k < left.cols(); ++k) {
                    sum += left[i, k] * right[k, j];
                }
                result[i, j] = sum;
            }
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_sum(Matrix<Value> const& left, Matrix<Value> const& right) -> Matrix<Value>
    {
        if (left.cols() != right.cols() || left.rows() != right.rows()) {
            throw Error{"Wrong dimensions\n"};
        }

        auto result{Matrix<Value>::zeros(left.rows(), left.cols())};
        for (Size i{}; i < left.rows(); ++i) {
            for (Size j{}; j < left.cols(); ++j) {
                result[i, j] = left[i, j] + right[i, j];
            }
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_difference(Matrix<Value> const& left, Matrix<Value> const& right)
        -> Matrix<Value>
    {
        if (left.cols() != right.cols() || left.rows() != right.rows()) {
            throw Error{"Wrong dimensions\n"};
        }

        auto result{Matrix<Value>::zeros(left.rows(), left.cols())};
        for (Size i{}; i < left.rows(); ++i) {
            for (Size j{}; j < left.cols(); ++j) {
                result[i, j] = left[i, j] - right[i, j];
            }
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_scale(Matrix<Value> const& matrix, Value const scale) -> Matrix<Value>
    {
        if (scale == std::numeric_limits<Value>::max()) {
            throw Error{"Multiplication by inf!\n"};
        }

        auto result{Matrix<Value>::zeros(matrix.rows(), matrix.cols())};
        for (Size i{}; i < matrix.rows(); ++i) {
            for (Size j{}; j < matrix.cols(); ++j) {
                result[i, j] = matrix[i, j] * scale;
            }
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto matrix_power(Matrix<Value> const& matrix, Value const power) -> Matrix<Value>
    {
        if (!matrix.is_square()) {
            throw Error{"Wrong dimensions\n"};
        }

        if (power == 1) {
            return matrix;
        }

        auto result{matrix};
        for (Size i{}; i < power; ++i) {
            result = matrix_product(result, matrix);
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator+(Matrix<Value> const& left, Matrix<Value> const& right) -> Matrix<Value>
    {
        try {
            return matrix_sum(left, right);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator-(Matrix<Value> const& left, Matrix<Value> const& right) -> Matrix<Value>
    {
        try {
            return matrix_difference(left, right);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator*(Value const scale, Matrix<Value> const& matrix) -> Matrix<Value>
    {
        try {
            return matrix_scale(matrix, scale);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator*(Matrix<Value> const& matrix, Value const scale) -> Matrix<Value>
    {
        try {
            return matrix_scale(matrix, scale);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator*(Matrix<Value> const& left, Matrix<Value> const& right) -> Matrix<Value>
    {
        try {
            return matrix_product(left, right);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator/(Matrix<Value> const& matrix, Value const scale) -> Matrix<Value>
    {
        try {
            return matrix_scale(matrix, 1 / scale);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator/(Matrix<Value> const& left, Matrix<Value> const& right) -> Matrix<Value>
    {
        try {
            return matrix_product(left, matrix_inverse(right));
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] constexpr auto operator^(Matrix<Value> const& matrix, Value const scale) -> Matrix<Value>
    {
        try {
            return matrix_power(matrix, scale);
        } catch (Error const& error) {
            throw error;
        }
    }

}; // namespace Linalg::Heap

#endif // HEAP_MATRIX_HPP