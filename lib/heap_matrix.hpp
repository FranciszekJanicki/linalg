#ifndef HEAP_MATRIX_HPP
#define HEAP_MATRIX_HPP

#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <expected>
#include <initializer_list>
#include <numeric>
#include <print>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Linalg::Heap {

    template <std::floating_point T>
    struct Matrix {
    public:
        using Vector = std::vector<T>;
        using Data = std::vector<std::vector<T>>;
        using VectorInit = std::initializer_list<T>;
        using Init = std::initializer_list<std::initializer_list<T>>;

        [[nodiscard]] static Data make_data(std::size_t const rows, std::size_t const cols)
        {
            Data data{};
            data.reserve(rows);
            for (std::size_t i{}; i < rows; ++i) {
                auto& column{data.emplace_back()};
                column.reserve(cols);
                for (std::size_t j{}; j < cols; ++j) {
                    column.emplace_back();
                }
            }
            return data;
        }

        [[nodiscard]] static Data make_data(Init const init)
        {
            Data data{};
            data.reserve(init.size());
            for (auto const row : init) {
                auto& column{data.emplace_back()};
                column.reserve(row.size());
                for (std::size_t j{}; j < row.size(); ++j) {
                    column.emplace_back();
                }
            }
            return data;
        }

        [[nodiscard]] static Matrix zeros(std::size_t const rows, std::size_t const cols)
        {
            return Matrix{make_data(rows, cols)};
        }

        [[nodiscard]] static Matrix ones(std::size_t const rows, std::size_t const cols)
        {
            Matrix result{make_data(rows, cols)};
            for (std::size_t i{}; i < rows; ++i) {
                for (std::size_t j{}; i < cols; ++j) {
                    result[i, j] = T{1};
                }
            }
            return result;
        }

        [[nodiscard]] static Matrix diagonal(VectorInit const init)
        {
            Matrix result{make_data(init.size(), init.size())};
            for (std::size_t i{}; i < init.size(); ++i) {
                for (std::size_t j{}; i < init.size(); ++j) {
                    result[i, j] = (i == j ? init[i] : static_cast<T>(0));
                }
            }
            return result;
        }

        [[nodiscard]] static Matrix eye(std::size_t const dimensions)
        {
            Matrix result{make_data(dimensions, dimensions)};
            for (std::size_t i{}; i < dimensions; ++i) {
                for (std::size_t j{}; i < dimensions; ++j) {
                    result[i, j] = (i == j ? T{1} : static_cast<T>(0));
                }
            }
            return result;
        }

        [[nodiscard]] static Matrix row(std::size_t const rows)
        {
            return Matrix{make_data(rows, 1)};
        }

        [[nodiscard]] static Matrix row(VectorInit const init)
        {
            Matrix result{make_data(init.size(), 1)};
            for (std::size_t i{}; i < init.size(); ++i) {
                result[i, 0] = *(init.begin() + i);
            }
            return result;
        }

        [[nodiscard]] static Matrix column(std::size_t const cols)
        {
            return Matrix{make_data(1, cols)};
        }

        [[nodiscard]] static Matrix column(VectorInit const init)
        {
            Matrix result{make_data(1, init.size())};
            for (std::size_t i{}; i < init.size(); ++i) {
                result[0, i] = *(init.begin() + i);
            }
            return result;
        }

        [[nodiscard]] Matrix() noexcept = default;

        [[nodiscard]] Matrix(Init const init) : data{make_data(init)}
        {}

        [[nodiscard]] Matrix(Data&& data) : data{std::forward<Data>(data)}
        {}

        [[nodiscard]] Matrix(std::size_t const rows, std::size_t const cols) : data{make_data(rows, cols)}
        {}

        [[nodiscard]] Matrix(Matrix const& other) = default;

        [[nodiscard]] Matrix(Matrix&& other) noexcept = default;

        [[nodiscard]] ~Matrix() noexcept = default;

        [[nodiscard]] Matrix& operator=(Matrix const& other) = default;

        [[nodiscard]] Matrix& operator=(Matrix&& other) noexcept = default;

        [[nodiscard]] Matrix& operator=(this Matrix& self, Init const init) noexcept
        {
            self.data = make_data(init);
        }

        [[nodiscard]] Matrix& operator+=(this Matrix& self, Matrix const& other)
        {
            try {
                self = matrix_sum(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Matrix& operator-=(this Matrix& self, Matrix const& other)
        {
            try {
                self = matrix_difference(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Matrix& operator*=(this Matrix& self, T const scale) noexcept
        {
            self = matrix_scale(self, scale);
            return self;
        }

        [[nodiscard]] Matrix& operator*=(this Matrix& self, Matrix const& other)
        {
            try {
                self = matrix_product(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Matrix& operator/=(this Matrix& self, T const scale)
        {
            try {
                self = matrix_scale(self, 1 / scale);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Matrix& operator/=(this Matrix& self, Matrix const& other)
        {
            try {
                self = matrix_product(self, matrix_inverse(other));
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Matrix& operator^=(this Matrix& self, T const scale)
        {
            try {
                self = matrix_power(self, scale);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Vector const& Matrix<T> operator[](this Matrix const& self, std::size_t const row)
        {
            if (row >= self.rows()) {
                throw std::runtime_error{"Wrong dimensions\n"};
            }
            return self.data[row];
        }

        [[nodiscard]] Vector& operator[](this Matrix& self, std::size_t const row)
        {
            if (row >= self.rows()) {
                throw std::runtime_error{"Wrong dimensions\n"};
            }
            return self.data[row];
        }

        [[nodiscard]] T& operator[](this Matrix& self, std::size_t const row, std::size_t const column)
        {
            if (row >= self.rows() || column >= self.cols()) {
                throw std::runtime_error{"Wrong dimensions\n"};
            }
            return self.data[row][column];
        }

        [[nodiscard]] T const& operator[](this Matrix const& self, std::size_t const row, std::size_t const column)

        {
            if (row >= self.rows() || column >= self.cols()) {
                throw std::runtime_error{"Wrong dimensions\n"};
            }
            return self.data[row][column];
        }

        [[nodiscard]] bool operator<=>(this Matrix const& self, Matrix const& other) noexcept = default;

        void print(this Matrix const& self) noexcept
        {
            std::print("[");
            if (!self.data.empty()) {
                for (auto& row : self.data) {
                    std::print("[");
                    if (!row.empty()) {
                        for (auto& col : row) {
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

        [[nodiscard]] bool is_square(this Matrix const& self) noexcept
        {
            return self.rows() == self.cols();
        }

        [[nodiscard]] std::size_t rows(this Matrix const& self) noexcept
        {
            return self.data.size();
        }

        [[nodiscard]] std::size_t cols(this Matrix const& self) noexcept
        {
            return self.data.front().size();
        }

        void transpose(this Matrix& self)
        {
            try {
                self = matrix_transpose(self);
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        void invert(this Matrix& self)
        {
            try {
                self = matrix_inverse(self);
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

    private:
        Data data{};
    };

    template <typename T>
    [[nodiscard]] Matrix<T>
    matrix_minor(Matrix<T> const& matrix, std::size_t const row, std::size_t const column, std::size_t const dimensions)

    {
        if (!matrix.is_square()) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }
        if (row >= dimensions || column >= dimensions) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }

        if (dimensions == 1) {
            return matrix;
        }

        auto minor{Matrix<T>::zeros(dimensions, dimensions)};
        std::size_t cof_i{};
        std::size_t cof_j{};
        for (std::size_t i{0}; i < dimensions; ++i) {
            for (std::size_t j{}; j < dimensions; ++j) {
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

    template <typename T>
    [[nodiscard]] T matrix_det(Matrix<T> const& matrix, std::size_t const dimensions)
    {
        if (!matrix.is_square()) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }
        if (matrix.rows() < dimensions || matrix.cols() < dimensions) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }

        if (dimensions == 1) {
            return matrix[0, 0];
        }
        if (dimensions == 2) {
            return (matrix[0, 0] * matrix[1, 1]) - (matrix[1, 0] * matrix[0, 1]);
        }

        try {
            T det{};
            for (std::size_t i{0}; i < dimensions; ++i) {
                det += (i % 2 == 0 ? 1 : -1) * matrix[0, i] *
                       matrix_det(matrix_minor(matrix, 0, i, dimensions), dimensions - 1);
            }
            return det;
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_transpose(Matrix<T> const& matrix)
    {
        auto result{Matrix<T>::zeros(matrix.rows(), matrix.cols())};
        for (std::size_t i{0}; i < matrix.rows(); ++i) {
            for (std::size_t j{0}; j < matrix.cols(); ++j) {
                result[i, j] = matrix[j, i];
            }
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_complement(Matrix<T> const& matrix)
    {
        if (!matrix.is_square()) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }

        std::size_t const dimensions{matrix.rows()};
        if (dimensions == 1) {
            return matrix;
        }

        auto result{Matrix<T>::zeros(dimensions, dimensions)};
        for (std::size_t i{}; i < dimensions; ++i) {
            for (std::size_t j{}; j < dimensions; j++) {
                try {
                    result[i, j] = ((i + j) % 2 == 0 ? 1 : -1) *
                                   matrix_det(matrix_minor(matrix, i, j, dimensions), dimensions - 1);
                } catch (std::runtime_error const& error) {
                    throw error;
                }
            }
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_adjoint(Matrix<T> const& matrix)
    {
        try {
            return matrix_transpose(matrix_complement(matrix));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_inverse(Matrix<T> const& matrix)
    {
        try {
            return matrix_scale(matrix_adjoint(matrix), 1 / matrix_det(matrix, matrix.rows()));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_upper_triangular(Matrix<T> const& matrix)
    {
        try {
            return matrix_transpose(matrix_lower_triangular(matrix));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_lower_triangular(Matrix<T> const& matrix)
    {
        if (!matrix.is_square()) {
            throw std::runtime_error{"Wrong dimensions!\n"};
        }

        if (matrix.rows() == 1) {
            return matrix;
        }

        auto result{Matrix<T>::zeros(matrix.rows(), matrix.cols())};
        for (std::size_t i{}; i < matrix.rows(); ++i) {
            for (std::size_t j{}; j <= i; ++j) {
                T sum{};
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

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_product(Matrix<T> const& left, Matrix<T> const& right)
    {
        auto result{Matrix<T>::zeros(left.rows(), right.rows())};
        for (std::size_t i{}; i < left.rows(); ++i) {
            for (std::size_t j{}; j < right.cols(); ++j) {
                T sum{};
                for (std::size_t k{}; k < left.cols(); ++k) {
                    sum += left[i, k] * right[k, j];
                }
                result[i, j] = sum;
            }
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_sum(Matrix<T> const& left, Matrix<T> const& right)
    {
        if (left.cols() != right.cols() || left.rows() != right.rows()) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }

        auto result{Matrix<T>::zeros(left.rows(), left.cols())};
        for (std::size_t i{}; i < left.rows(); ++i) {
            for (std::size_t j{}; j < left.cols(); ++j) {
                result[i, j] = left[i, j] + right[i, j];
            }
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_difference(Matrix<T> const& left, Matrix<T> const& right)
    {
        if (left.cols() != right.cols() || left.rows() != right.rows()) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }

        auto result{Matrix<T>::zeros(left.rows(), left.cols())};
        for (std::size_t i{}; i < left.rows(); ++i) {
            for (std::size_t j{}; j < left.cols(); ++j) {
                result[i, j] = left[i, j] - right[i, j];
            }
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_scale(Matrix<T> const& matrix, T const scale) noexcept
    {
        auto result{Matrix<T>::zeros(matrix.rows(), matrix.cols())};
        for (std::size_t i{}; i < matrix.rows(); ++i) {
            for (std::size_t j{}; j < matrix.cols(); ++j) {
                result[i, j] = matrix[i, j] * scale;
            }
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> matrix_power(Matrix<T> const& matrix, T const power)
    {
        if (!matrix.is_square()) {
            throw std::runtime_error{"Wrong dimensions\n"};
        }

        if (power == 1) {
            return matrix;
        }

        auto result{matrix};
        for (std::size_t i{}; i < power; ++i) {
            result = matrix_product(result, matrix);
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator+(Matrix<T> const& left, Matrix<T> const& right)
    {
        try {
            return matrix_sum(left, right);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator-(Matrix<T> const& left, Matrix<T> const& right)
    {
        try {
            return matrix_difference(left, right);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator*(T const scale, Matrix<T> const& matrix) noexcept
    {
        return matrix_scale(matrix, scale);
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator*(Matrix<T> const& matrix, T const scale) noexcept
    {
        return matrix_scale(matrix, scale);
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator*(Matrix<T> const& left, Matrix<T> const& right)
    {
        try {
            return matrix_product(left, right);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator/(Matrix<T> const& matrix, T const scale) noexcept
    {
        if (scale == static_cast<T>(0)) {
            throw std::runtime_error{"Division by 0!\n"};
        }
        return matrix_scale(matrix, 1 / scale);
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator/(Matrix<T> const& left, Matrix<T> const& right)
    {
        try {
            return matrix_product(left, matrix_inverse(right));
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Matrix<T> operator^(Matrix<T> const& matrix, T const scale)
    {
        try {
            return matrix_power(matrix, scale);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

}; // namespace Linalg::Heap

#endif // HEAP_MATRIX_HPP