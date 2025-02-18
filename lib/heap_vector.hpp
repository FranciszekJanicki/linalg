#ifndef HEAP_VECTOR_HPP
#define HEAP_VECTOR_HPP

#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <exception>
#include <initializer_list>
#include <print>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Linalg::Heap {

    template <std::floating_point T>
    struct Vector {
    public:
        using Data = std::vector<T>;
        using Init = std::initializer_list<T>;

        [[nodiscard]] static Data make_data(Init const init)
        {
            // Data data{};

            // data.reserve(init.size());
            // for (auto const elem : init) {
            //     data.push_back(elem);
            // }

            // return data;
            return Data{init};
        }

        [[nodiscard]] static Data make_data(std::size_t const elems)
        {
            // Data data{};

            // data.reserve(elems);
            // for (std::size_t i{}; i < elems; ++i) {
            //     data.emplace_back();
            // }

            // return data;
            return Data(elems, static_cast<T>(0));
        }

        [[nodiscard]] Vector zeros(std::size_t const elems)
        {
            // return Vector{make_data(elems)};
            return Data(elems, static_cast<T>(0));
        }

        [[nodiscard]] Vector ones(std::size_t const elems)
        {
            // Vector result{make_data(elems)};

            // for (auto& elem : result) {
            //     elem = T{1};
            // }

            // return result;
            return Data(elems, T{1});
        }

        [[nodiscard]] Vector() noexcept = default;

        [[nodiscard]] Vector(Data&& data) : data{std::forward<Data>(data)}
        {}

        [[nodiscard]] Vector(Init const init) : data{init}
        {}

        [[nodiscard]] Vector(std::size_t const elems) : data{make_data(elems)}
        {}

        [[nodiscard]] Vector(Vector const& other) = default;

        [[nodiscard]] Vector(Vector&& other) noexcept = default;

        [[nodiscard]] ~Vector() noexcept = default;

        [[nodiscard]] Vector& operator=(Vector const& other) = default;

        [[nodiscard]] Vector& operator=(Vector&& other) noexcept = default;

        Vector& operator=(this Vector& self, Init const init)
        {
            self.data = data;
            return self;
        }

        [[nodiscard]] Vector& operator+=(this Vector& self, Vector const& other)
        {
            try {
                self = vector_sum(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Vector& operator-=(this Vector& self, Vector const& other)
        {
            try {
                self = vector_difference(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] Vector& operator*=(this Vector& self, T const scale)
        {
            self = vector_scale(self, scale);
            return self;
        }

        [[nodiscard]] Vector& operator/=(this Vector& self, T const scale)
        {
            if (scale == static_cast<T>(0)) {
                throw std::runtime_error{"Division by 0!"};
            }
            self = vector_scale(self, 1 / scale);
            return self;
        }

        [[nodiscard]] T& operator[](this Vector& self, std::size_t const elem) ->
        {
            if (elem > self.elems()) {
                throw std::runtime_error{"Wrong dimensions"};
            }
            return self.data[elem];
        }

        [[nodiscard]] T auto operator[](this Vector const& self, std::size_t const elem) noexcept const&
        {
            if (elem > self.elems()) {
                throw std::runtime_error{"Wrong dimensions"};
            }
            return self.data[elem];
        }

        [[nodiscard]] bool operator<=>(this Vector const& self, Vector const& other) noexcept = default;

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

        [[nodiscard]] std::size_t elems(this Vector const& self) noexcept
        {
            return self.data.size();
        }

        Data data{};
    };

    template <typename T>
    [[nodiscard]] Vector<T> vector_sum(Vector<T> const& left, Vector<T> const& right)
    {
        if (left.elems() != right.elems()) {
            throw std::runtime_error{"Incorrect dimensions!"};
        }

        auto result{Vector<T>::make_zeros(left.elems())};
        for (std::size_t i{0}; i < left.elems(); ++i) {
            result[i] = left[i] + right[i];
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Vector<T> vector_difference(Vector<T> const& left, Vector<T> const& right)
    {
        if (left.elems() != right.elems()) {
            throw std::runtime_error{"Incorrect dimensions!"};
        }

        auto result{Vector<T>::make_zeros(left.elems())};
        for (std::size_t i{0}; i < left.elems(); ++i) {
            result[i] = left[i] - right[i];
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Vector<T> vector_scale(Vector<T> const& vector, T const scale) noexcept
    {
        auto result{Vector<T>::make_zeros(vector.elems())};
        for (std::size_t i{0}; i < vector.elems(); ++i) {
            result[i] = vector[i] * scale;
        }
        return result;
    }

    template <typename T>
    [[nodiscard]] Vector<T> operator+(Vector<T> const& left, Vector<T> const& right)
    {
        try {
            return vector_sum(left, right);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Vector<T> operator-(Vector<T> const& left, Vector<T> const& right)
    {
        try {
            return vector_difference(left, right);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename T>
    [[nodiscard]] Vector<T> operator*(T const scale, Vector<T> const& vector) noexcept
    {
        return vector_scale(vector, scale);
    }

    template <typename T>
    [[nodiscard]] Vector<T> operator*(Vector<T> const& vector, T const scale) noexcept
    {
        return vector_scale(vector, scale);
    }

    template <typename T>
    [[nodiscard]] Vector<T> operator/(Vector<T> const& vector, T const scale)
    {
        if (scale == static_cast<T>(0)) {
            throw std::runtime_error{"Division by 0!"};
        }
        return vector_scale(vector, 1 / scale);
    }

}; // namespace Linalg::Heap

#endif // HEAP_VECTOR_HPP