#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

#include "common.hpp"
#include "quaternion3d.hpp"
#include <cmath>
#include <compare>
#include <exception>
#include <stdexcept>
#include <utility>

using Error = std::runtime_error;

namespace Linalg {

    template <Arithmetic Value>
    struct Vector3D {
        [[nodiscard]] constexpr auto distance(this Vector3D const& self, Vector3D const& other) noexcept -> Value
        {
            return std::sqrt(std::pow(self.x - other.x, 2) + std::pow(self.y - other.y, 2) +
                             std::pow(self.z - other.z, 2));
        }

        [[nodiscard]] constexpr auto magnitude(this Vector3D const& self) noexcept -> Value
        {
            return std::sqrt(std::pow(self.x, 2) + std::pow(self.y, 2) + std::pow(self.z, 2));
        }

        [[nodiscard]] constexpr auto rotated(this Vector3D const& self, Quaternion3D<Value> const& quaternion) noexcept
            -> Vector3D
        {
            Quaternion3D p(0, self.x, self.y, self.z);
            p *= quaternion;
            p *= quaternion.conjugated();
            return Vector3D{p.x, p.y, p.z};
        }

        constexpr auto rotate(this Vector3D& self, Quaternion3D<Value> const& quaternion) noexcept -> void
        {
            Quaternion3D p(0, self.x, self.y, self.z);
            p *= quaternion;
            p *= quaternion.conjugated();
            self.x = p.x;
            self.y = p.y;
            self.z = p.z;
        }

        [[nodiscard]] constexpr auto normalized(this Vector3D const& self) noexcept -> Vector3D
        {
            const auto im{Value{1} / self.magnitude()};
            return Vector3D{self.x * im, self.y * im, self.z * im};
        }

        constexpr auto normalize(this Vector3D& self) noexcept -> void
        {
            const auto im{Value{1} / self.magnitude()};
            self *= im;
        }

        [[nodiscard]] constexpr auto operator+=(this Vector3D& self, Vector3D const& other) noexcept -> Vector3D&
        {
            self.x += other.x;
            self.y += other.y;
            self.z += other.z;
            return self;
        }

        [[nodiscard]] constexpr auto operator-=(this Vector3D& self, Vector3D const& other) noexcept -> Vector3D&
        {
            self.x -= other.x;
            self.y -= other.y;
            self.z -= other.z;
            return self;
        }

        [[nodiscard]] constexpr auto operator*=(this Vector3D& self, Value const factor) -> Vector3D&
        {
            if (factor == std::numeric_limits<Value>::max()) {
                throw Error{"Multiplication by inf\n"};
            }

            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        [[nodiscard]] constexpr auto operator/=(this Vector3D& self, Value const factor) -> Vector3D&
        {
            if (factor == 0) {
                throw Error{"Disivion by 0\n"};
            }

            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        template <Arithmetic Converted>
        [[nodiscard]] explicit constexpr operator Vector3D<Converted>(this Vector3D const& self) noexcept
        {
            return Vector3D<Converted>{static_cast<Converted>(self.x),
                                       static_cast<Converted>(self.y),
                                       static_cast<Converted>(self.z)};
        }

        [[nodiscard]] constexpr bool operator<=>(this Vector3D const& self, Vector3D const& other) noexcept = default;

        Value x{};
        Value y{};
        Value z{};
    };

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto operator+(Vector3D<Value> const& left, Vector3D<Value> const& right) noexcept
    {
        return Vector3D<Value>{left.x + right.x, left.y + right.y, left.z + right.z};
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto operator-(Vector3D<Value> const& left, Vector3D<Value> const& right) noexcept
    {
        return Vector3D<Value>{left.x - right.x, left.y - right.y, left.z - right.z};
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto operator*(Value const factor, Vector3D<Value> const& vector)
    {
        if (factor == std::numeric_limits<Value>::max()) {
            throw Error{"Multiplication by inf\n"};
        }

        return Vector3D<Value>{vector.x * factor, vector.y * factor, vector.z * factor};
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto operator*(Vector3D<Value> const& vector, Value const factor)
    {
        try {
            return factor * vector;
        } catch (Error const& error) {
            throw error;
        }
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto operator/(Vector3D<Value> const& vector, Value const factor)
    {
        if (factor == 0) {
            throw Error{"Division by zero\n"};
        }

        return Vector3D<Value>{vector.x / factor, vector.y / factor, vector.z / factor};
    }

}; // namespace Linalg

#endif // VECTOR3D_HPP