#ifndef STACK_VECTOR_HPP
#define STACK_VECTOR_HPP

#include "common.hpp"
#include <array>
#include <cstddef>
#include <exception>
#include <fmt/core.h>
#include <stdexcept>
#include <utility>

namespace Linalg::Stack {

    template <Arithmetic Value, Size ELEMS>
    struct Vector {
        using Data = std::array<Value, ELEMS>;

        [[nodiscard]] constexpr auto operator[](this Vector const& self, Size const elem) -> Value const&
        {
            if (elem >= ELEMS) {
                throw Error{"Out of bounds\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] constexpr auto operator[](this Vector& self, Size const elem) -> Value&
        {
            if (elem >= ELEMS) {
                throw Error{"Out of bounds\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] constexpr auto size(this Vector const& self) noexcept -> Size
        {
            return ELEMS;
        }

        [[nodiscard]] constexpr auto operator+=(this Vector& self, Vector const& other) noexcept -> Vector&
        {
            self = sum(self, other);
            return self;
        }

        [[nodiscard]] constexpr auto operator-=(this Vector& self, Vector const& other) noexcept -> Vector&
        {
            self = difference(self, other);
            return self;
        }

        [[nodiscard]] constexpr auto operator*=(this Vector& self, Value const scale) -> Vector&
        {
            try {
                self = scale(self, scale);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] constexpr auto operator/=(this Vector& self, Value const scale) noexcept -> Vector&
        {
            try {
                self = scale(self, 1 / scale);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        constexpr auto print(this Vector const& self) noexcept -> void
        {
            fmt::print("[");
            for (auto& elem : self.data) {
                fmt::print("{}", elem);
                if (elem != self.data.back()) {
                    fmt::print(", ");
                }
            }
            fmt::print("]\n");
        }

        Data data{};
    };

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto vector_sum(Vector<Value, ELEMS> const& left,
                                            Vector<Value, ELEMS> const& right) noexcept -> Vector<Value, ELEMS>
    {
        Vector<Value, ELEMS> result;
        for (Size i{}; i < ELEMS; ++i) {
            result[i] = left[i] + right[i];
        }
        return result;
    }

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto vector_difference(Vector<Value, ELEMS> const& left,
                                                   Vector<Value, ELEMS> const& right) noexcept -> Vector<Value, ELEMS>
    {
        Vector<Value, ELEMS> result;
        for (Size i{}; i < ELEMS; ++i) {
            result[i] = left[i] - right[i];
        }
        return result;
    }

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto vector_scale(Vector<Value, ELEMS> const& vector, Vector<Value, ELEMS> const scale)
        -> Vector<Value, ELEMS>
    {
        if (scale == std::numeric_limits<Value>::max()) {
            throw Error{"Multiplication by inf!\n"};
        }

        Vector<Value, ELEMS> result;
        for (Size i{}; i < ELEMS; ++i) {
            result[i] = vector[i] * scale;
        }
        return result;
    }

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto operator+(Vector<Value, ELEMS> const& left, Vector<Value, ELEMS> const& right) noexcept
        -> Vector<Value, ELEMS>
    {
        return vector_sum(left, right);
    }

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto operator-(Vector<Value, ELEMS> const& left, Vector<Value, ELEMS> const& right) noexcept
        -> Vector<Value, ELEMS>
    {
        return vector_difference(left, right);
    }

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto operator*(Value const scale, Vector<Value, ELEMS> const& vector)
        -> Vector<Value, ELEMS>
    {
        try {
            return vector_scale(vector, scale);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto operator*(Vector<Value, ELEMS> const& vector, Value const scale)
        -> Vector<Value, ELEMS>
    {
        try {
            return vector_scale(vector, scale);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <Arithmetic Value, Size ELEMS>
    [[nodiscard]] constexpr auto operator/(Vector<Value, ELEMS> const& vector, Value const scale)
        -> Vector<Value, ELEMS>
    {
        try {
            return vector_scale(vector, 1 / scale);
        } catch (Error const& error) {
            throw error;
        }
    }

}; // namespace Linalg::Stack

#endif // STACK_VECTOR_HPP