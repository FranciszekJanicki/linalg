#ifndef STACK_VECTOR_HPP
#define STACK_VECTOR_HPP

#include <array>
#include <concepts>
#include <cstddef>
#include <exception>
#include <print>
#include <stdexcept>
#include <utility>

namespace Linalg::Stack {

    template <std::floating_point T, std::size_t N>
    struct Vector {
        [[nodiscard]] T const& operator[](this Vector const& self, std::size_t const elem)
        {
            if (elem >= N) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] T& operator[](this Vector& self, std::size_t const elem)
        {
            if (elem >= N) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] std::size_t size(this Vector const& self) noexcept
        {
            return N;
        }

        [[nodiscard]] Vector& operator+=(this Vector& self, Vector const& other) noexcept
        {
            self = vector_sum(self, other);
            return self;
        }

        [[nodiscard]] Vector& operator-=(this Vector& self, Vector const& other) noexcept
        {
            self = vector_difference(self, other);
            return self;
        }

        [[nodiscard]] Vector& operator*=(this Vector& self, T const scale) noexcept
        {
            self = vector_scale(self, scale);
            return self;
        }

        [[nodiscard]] Vector& operator/=(this Vector& self, T const scale)
        {
            if (scale == static_cast<T>(0)) {
                throw std::runtime_error{"Division by 0!\n"};
            }
            self = vector_scale(self, 1 / scale);
            return self;
        }

        void print(this Vector const& self) noexcept
        {
            std::print("[");
            for (auto& elem : self.data) {
                std::print("{}", elem);
                if (elem != self.data.back()) {
                    std::print(", ");
                }
            }
            std::print("]\n");
        }

        std::array<T, N> data{};
    };

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> vector_sum(Vector<T, N> const& left, Vector<T, N> const& right) noexcept

    {
        Vector<T, N> result;
        for (std::size_t i{}; i < N; ++i) {
            result[i] = left[i] + right[i];
        }
        return result;
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> vector_difference(Vector<T, N> const& left, Vector<T, N> const& right) noexcept

    {
        Vector<T, N> result;
        for (std::size_t i{}; i < N; ++i) {
            result[i] = left[i] - right[i];
        }
        return result;
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> vector_scale(Vector<T, N> const& vector, Vector<T, N> const scale) noexcept

    {
        Vector<T, N> result;
        for (std::size_t i{}; i < N; ++i) {
            result[i] = vector[i] * scale;
        }
        return result;
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> operator+(Vector<T, N> const& left, Vector<T, N> const& right) noexcept

    {
        return vector_sum(left, right);
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> operator-(Vector<T, N> const& left, Vector<T, N> const& right) noexcept

    {
        return vector_difference(left, right);
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> operator*(T const scale, Vector<T, N> const& vector) noexcept
    {
        return vector_scale(vector, scale);
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> operator*(Vector<T, N> const& vector, T const scale) noexcept
    {
        return vector_scale(vector, scale);
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] Vector<T, N> operator/(Vector<T, N> const& vector, T const scale)
    {
        if (scale == static_cast<T>(0)) {
            throw std::runtime_error{"Division by 0!\n"};
        }

        return vector_scale(vector, 1 / scale);
    }

}; // namespace Linalg::Stack

#endif // STACK_VECTOR_HPP