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

    template <std::floating_point Value>
    struct Vector {
    public:
        using Data = std::vector<Value>;
        using Init = std::initializer_list<Value>;

        [[nodiscard]] static auto make_data(Init const init) -> Data
        {
            // Data data{};

            // data.reserve(init.size());
            // for (auto const elem : init) {
            //     data.push_back(elem);
            // }

            // return data;
            return Data{init};
        }

        [[nodiscard]] static auto make_data(std::size_t const elems) -> Data
        {
            // Data data{};

            // data.reserve(elems);
            // for (std::size_t i{}; i < elems; ++i) {
            //     data.emplace_back();
            // }

            // return data;
            return Data(elems, Value{0});
        }

        [[nodiscard]] auto zeros(std::size_t const elems) -> Vector
        {
            // return Vector{make_data(elems)};
            return Data(elems, Value{0});
        }

        [[nodiscard]] auto ones(std::size_t const elems) -> Vector
        {
            // Vector result{make_data(elems)};

            // for (auto& elem : result) {
            //     elem = Value{1};
            // }

            // return result;
            return Data(elems, Value{1});
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

        [[nodiscard]] auto operator=(Vector const& other) -> Vector& = default;

        [[nodiscard]] auto operator=(Vector&& other) noexcept -> Vector& = default;

        auto operator=(this Vector& self, Init const init) -> Vector&
        {
            self.data = data;
            return self;
        }

        [[nodiscard]] auto operator+=(this Vector& self, Vector const& other) -> Vector&
        {
            try {
                self = vector_sum(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] auto operator-=(this Vector& self, Vector const& other) -> Vector&
        {
            try {
                self = vector_difference(self, other);
                return self;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        [[nodiscard]] auto operator*=(this Vector& self, Value const scale) -> Vector&
        {
         
                self = vector_scale(self, scale);
                return self;
        
        }

        [[nodiscard]] auto operator/=(this Vector& self, Value const scale) -> Vector&
        {
                  if (scale == Value{0.0}) {
            throw std::runtime_error{"Division by 0!\n"};
        }
                self = vector_scale(self, 1 / scale);
                return self;
           
        }

        [[nodiscard]] Valueauto operator[](this Vector& self, std::size_t const elem)->&
        {
            if (elem > self.elems()) {
                throw std::runtime_error{"Wrong dimensions\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] Value auto operator[](this Vector const& self, std::size_t const elem) noexcept -> const&
        {
            if (elem > self.elems()) {
                throw std::runtime_error{"Wrong dimensions\n"};
            }
            return self.data[elem];
        }

        [[nodiscard]] bool operator<=>(this Vector const& self, Vector const& other) noexcept = default;

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

        [[nodiscard]] auto elems(this Vector const& self) noexcept -> std::size_t
        {
            return self.data.size();
        }

        Data data{};
    };

    template <typename Value>
    [[nodiscard]] auto vector_sum(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        if (left.elems() != right.elems()) {
            throw std::runtime_error{"Incorrect dimensions!\n"};
        }

        auto result{Vector<Value>::make_zeros(left.elems())};
        for (std::size_t i{0}; i < left.elems(); ++i) {
            result[i] = left[i] + right[i];
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] auto vector_difference(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        if (left.elems() != right.elems()) {
            throw std::runtime_error{"Incorrect dimensions!\n"};
        }

        auto result{Vector<Value>::make_zeros(left.elems())};
        for (std::size_t i{0}; i < left.elems(); ++i) {
            result[i] = left[i] - right[i];
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] auto vector_scale(Vector<Value> const& vector, Value const scale) noexcept -> Vector<Value>
    {

        auto result{Vector<Value>::make_zeros(vector.elems())};
        for (std::size_t i{0}; i < vector.elems(); ++i) {
            result[i] = vector[i] * scale;
        }
        return result;
    }

    template <typename Value>
    [[nodiscard]] auto operator+(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        try {
            return vector_sum(left, right);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] auto operator-(Vector<Value> const& left, Vector<Value> const& right) -> Vector<Value>
    {
        try {
            return vector_difference(left, right);
        } catch (std::runtime_error const& error) {
            throw error;
        }
    }

    template <typename Value>
    [[nodiscard]] auto operator*(Value const scale, Vector<Value> const& vector) noexcept -> Vector<Value>
    {
     
            return vector_scale(vector, scale);

        
    }

    template <typename Value>
    [[nodiscard]] auto operator*(Vector<Value> const& vector, Value const scale)noexcept -> Vector<Value>
    {
      
            return vector_scale(vector, scale);
     
    }

    template <typename Value>
    [[nodiscard]] auto operator/(Vector<Value> const& vector, Value const scale) -> Vector<Value>
    {
             if (scale == Value{0.0}) {
            throw std::runtime_error{"Division by 0!\n"};
        }
            return vector_scale(vector, 1 / scale);
      
    }

}; // namespace Linalg::Heap

#endif // HEAP_VECTOR_HPP