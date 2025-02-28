#ifndef HEAP_VECTOR_HPP
#define HEAP_VECTOR_HPP

#include "common.hpp"
#include <cassert>
#include <cmath>
#include <compare>
#include <exception>
#include <fmt/core.h>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Linalg::Heap {

    template <Arithmetic Value>
    struct Vector {
    public:
        using Data = std::vector<Value>;
        using Init = std::initializer_list<Value>;

        [[nodiscard]] static inline auto make_data(Init const init) -> Data
        {
            // Data data{};

            // data.reserve(init.size());
            // for (auto const elem : init) {
            //     data.push_back(elem);
            // }

            // return data;
            return Data{init};
        }

        [[nodiscard]] static inline auto make_data(Size const elems) -> Data
        {
            // Data data{};

            // data.reserve(elems);
            // for (Size i{}; i < elems; ++i) {
            //     data.emplace_back();
            // }

            // return data;
            return Data(elems, Value{0});
        }

        [[nodiscard]] inline auto zeros(Size const elems) -> Vector
        {
            // return Vector{make_data(elems)};
            return Data(elems, Value{0});
        }

        [[nodiscard]] inline auto ones(Size const elems) -> Vector
        {
            // Vector result{make_data(elems)};

            // for (auto& elem : result) {
            //     elem = Value{1};
            // }

            // return result;
            return Data(elems, Value{1});
        }

        [[nodiscard]] inline Vector() noexcept = default;

        [[nodiscard]] inline Vector(Data&& data) : data{std::forward<Data>(data)}
        {}

        [[nodiscard]] inline Vector(Init const init) : data{init}
        {}

        [[nodiscard]] inline Vector(Size const elems) : data{make_data(elems)}
        {}

        [[nodiscard]] inline Vector(Vector const& other) = default;

        [[nodiscard]] inline Vector(Vector&& other) noexcept = default;

        [[nodiscard]] inline ~Vector() noexcept = default;

        [[nodiscard]] inline auto operator=(Vector const& other) -> Vector& = default;

        [[nodiscard]] inline auto operator=(Vector&& other) noexcept -> Vector& = default;

        inline auto operator=(this Vector& self, Init const init) -> Vector&
        {
            self.data = data;
            return self;
        }

        [[nodiscard]] inline auto operator+=(this Vector& self, Vector const& other) -> Vector&
        {
            try {
                self = vector_sum(self, other);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] inline auto operator-=(this Vector& self, Vector const& other) -> Vector&
        {
            try {
                self = vector_difference(self, other);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] inline auto operator*=(this Vector& self, Value const scale) -> Vector&
        {
            try {
                self = vector_scale(self, scale);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] inline auto operator/=(this Vector& self, Value const scale) -> Vector&
        {
            try {
                self = vector_scale(self, 1 / scale);
                return self;
            } catch (Error const& error) {
                throw error;
            }
        }

        [[nodiscard]] inline Valueauto operator[](this Vector& self, Size const elem)->&
        {
            if (elem > self.elems()) {
                throw Error{"Wrong dimensions\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] inline Value auto operator[](this Vector const& self, Size const elem) noexcept -> const&
        {
            if (elem > self.elems()) {
                throw Error{"Wrong dimensions\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] inline bool operator<=>(this Vector const& self, Vector const& other) noexcept = default;

        inline auto print(this Vector const& self) noexcept -> void
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

        [[nodiscard]] inline auto elems(this Vector const& self) noexcept -> Size
        {
            return self.data.size();
        }

        Data data{};
    };

    template <typename Value>
    [[nodiscard]] inline auto vector_sum(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        if (left.elems() != right.elems()) {
            throw Error{"Incorrect dimensions!\n"};
        }

        auto result{Vector<Value>::make_zeros(left.elems())};
        for (Size i{0}; i < left.elems(); ++i) {
            result[i] = left[i] + right[i];
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] inline auto vector_difference(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        if (left.elems() != right.elems()) {
            throw Error{"Incorrect dimensions!\n"};
        }

        auto result{Vector<Value>::make_zeros(left.elems())};
        for (Size i{0}; i < left.elems(); ++i) {
            result[i] = left[i] - right[i];
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] inline auto vector_scale(Vector<Value> const& vector, Value const scale) -> Vector<Value>
    {
        if (scale == std::numeric_limits<Value>::max()) {
            throw Error{"Multiplication by inf!\n"};
        }

        auto result{Vector<Value>::make_zeros(vector.elems())};
        for (Size i{0}; i < vector.elems(); ++i) {
            result[i] = vector[i] * scale;
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] inline auto operator+(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        try {
            return vector_sum(left, right);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] inline auto operator-(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        try {
            return vector_difference(left, right);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] inline auto operator*(Value const scale, Vector<Value> const& vector) -> Vector<Value>
    {
        try {
            return vector_scale(vector, scale);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] inline auto operator*(Vector<Value> const& vector, Value const scale) -> Vector<Value>
    {
        try {
            return vector_scale(vector, scale);
        } catch (Error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] inline auto operator/(Vector<Value> const& vector, Value const scale) -> Vector<Value>
    {
        try {
            return vector_scale(vector, 1 / scale);
        } catch (Error const& error) {
            throw error;
        }
    }

}; // namespace Linalg::Heap

#endif // HEAP_VECTOR_HPP