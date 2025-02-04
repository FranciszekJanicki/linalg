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

    template <std::floating_point Value, std::size_t ELEMS>
    struct Vector {
        using Data = std::array<Value, ELEMS>;

        [[nodiscard]] auto operator[](this Vector const& self, std::size_t const elem) -> Value const&
        {
            if (elem >= ELEMS) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] auto operator[](this Vector& self, std::size_t const elem) -> Value&
        {
            if (elem >= ELEMS) {
                throw std::runtime_error{"Out of bounds\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] auto size(this Vector const& self) noexcept -> std::size_t
        {
            return ELEMS;
        }

        [[nodiscard]] auto operator+=(this Vector& self, Vector const& other) noexcept -> Vector&
        {
            self = vector_sum(self, other);
            return self;
        }

        [[nodiscard]] auto operator-=(this Vector& self, Vector const& other) noexcept -> Vector&
        {
            self = vector_difference(self, other);
            return self;
        }

        [[nodiscard]] auto operator*=(this Vector& self, Value const scale) noexcept-> Vector&
        {
                self = vector_scale(self, scale);
                return self;
         }

        [[nodiscard]] auto operator/=(this Vector& self, Value const scale)  -> Vector&
        {
                    if (scale == Value{0.0}) {
            throw std::runtime_error{"Division by 0!\n"};
        }
                self = vector_scale(self, 1 / scale);
                return self;
          
        }

        auto print(this Vector const& self) noexcept -> void
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

        Data data{};
    };

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto vector_sum(Vector<Value, ELEMS> const& left, Vector<Value, ELEMS> const& right) noexcept
        -> Vector<Value, ELEMS>
    {
        Vector<Value, ELEMS> result;
        for (std::size_t i{}; i < ELEMS; ++i) {
            result[i] = left[i] + right[i];
        }
        return result;
    }

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto vector_difference(Vector<Value, ELEMS> const& left, Vector<Value, ELEMS> const& right) noexcept
        -> Vector<Value, ELEMS>
    {
        Vector<Value, ELEMS> result;
        for (std::size_t i{}; i < ELEMS; ++i) {
            result[i] = left[i] - right[i];
        }
        return result;
    }

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto vector_scale(Vector<Value, ELEMS> const& vector, Vector<Value, ELEMS> const scale)
       noexcept -> Vector<Value, ELEMS>
    {
        Vector<Value, ELEMS> result;
        for (std::size_t i{}; i < ELEMS; ++i) {
            result[i] = vector[i] * scale;
        }
        return result;
    }

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto operator+(Vector<Value, ELEMS> const& left, Vector<Value, ELEMS> const& right) noexcept
        -> Vector<Value, ELEMS>
    {
        return vector_sum(left, right);
    }

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto operator-(Vector<Value, ELEMS> const& left, Vector<Value, ELEMS> const& right) noexcept
        -> Vector<Value, ELEMS>
    {
        return vector_difference(left, right);
    }

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto operator*(Value const scale, Vector<Value, ELEMS> const& vector) noexcept -> Vector<Value, ELEMS>
    {
       return vector_scale(vector, scale);
     
    }

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto operator*(Vector<Value, ELEMS> const& vector, Value const scale)noexcept -> Vector<Value, ELEMS>
    {
      
            return vector_scale(vector, scale);
   
    }

    template <std::floating_point Value, std::size_t ELEMS>
    [[nodiscard]] auto operator/(Vector<Value, ELEMS> const& vector, Value const scale) -> Vector<Value, ELEMS>
    {
        if (scale == Value{0.0}) {
            throw std::runtime_error{"Division by 0!\n"};
        }

        return vector_scale(vector, 1 / scale);
    }

}; // namespace Linalg::Stack

#endif // STACK_VECTOR_HPP