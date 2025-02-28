#ifndef ROTATION3D_HPP
#define ROTATION3D_HPP

#include "common.hpp"
#include "vector3d.hpp"
#include <array>
#include <cmath>
#include <compare>
#include <cstdlib>
#include <utility>

namespace Linalg {

    template <Arithmetic Value>
    struct Rotation3D {
        [[nodiscard]] inline auto operator+=(this Rotation3D& self, Rotation3D const& other) noexcept -> Rotation3D&
        {
            self.x += other.x;
            self.y += other.y;
            self.z += other.z;
            return self;
        }

        [[nodiscard]] inline auto operator-=(this Rotation3D& self, Rotation3D const& other) noexcept -> Rotation3D&
        {
            self.x -= other.x;
            self.y -= other.y;
            self.z -= other.z;
            return self;
        }

        [[nodiscard]] inline auto operator*=(this Rotation3D& self, Rotation3D const& other) noexcept -> Rotation3D&
        {
            self.x = Vector3D<Value>{self.x * other.x + self.y * other.x + self.z * other.x,
                                     self.x * other.y + self.y * other.y + self.z * other.y,
                                     self.x * other.z + self.y * other.z + self.z * other.z};
            self.y = Vector3D<Value>{self.x * other.x + self.y * other.x + self.z * other.x,
                                     self.x * other.y + self.y * other.y + self.z * other.y,
                                     self.x * other.z + self.y * other.z + self.z * other.z};
            self.z = Vector3D<Value>{self.x * other.x + self.y * other.x + self.z * other.x,
                                     self.x * other.y + self.y * other.y + self.z * other.y,
                                     self.x * other.z + self.y * other.z + self.z * other.z};
            return self;
        }

        [[nodiscard]] inline auto operator*=(this Rotation3D& self, Value const factor) noexcept -> Rotation3D&
        {
            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        [[nodiscard]] inline auto operator/=(this Rotation3D& self, Value const factor) noexcept -> Rotation3D&
        {
            self.x /= factor;
            self.y /= factor;
            self.z /= factor;
            return self;
        }

        [[nodiscard]] inline bool operator<=>(this Rotation3D const& self, Rotation3D const& other) noexcept = default;

        Vector3D<Value> x{};
        Vector3D<Value> y{};
        Vector3D<Value> z{};
    };

    template <Arithmetic Value>
    [[nodiscard]] inline auto operator+(Rotation3D<Value> const& left,
                                        Rotation3D<Value> const& right) noexcept -> Rotation3D<Value>
    {
        return Rotation3D<Value>{left.x + right.x, left.y + right.y, left.z + right.z};
    }

    template <Arithmetic Value>
    [[nodiscard]] inline auto operator-(Rotation3D<Value> const& left,
                                        Rotation3D<Value> const& right) noexcept -> Rotation3D<Value>
    {
        return Rotation3D<Value>{left.x - right.x, left.y - right.y, left.z - right.z};
    }

    template <Arithmetic Value>
    [[nodiscard]] inline auto operator*(Rotation3D<Value> const& left,
                                        Rotation3D<Value> const& right) noexcept -> Rotation3D<Value>
    {
        return Rotation3D<Value>{Vector3D<Value>{left.x * right.x + left.y * right.x + left.z * right.x,
                                                 left.x * right.y + left.y * right.y + left.z * right.y,
                                                 left.x * right.z + left.y * right.z + left.z * right.z},
                                 Vector3D<Value>{left.x * right.x + left.y * right.x + left.z * right.x,
                                                 left.x * right.y + left.y * right.y + left.z * right.y,
                                                 left.x * right.z + left.y * right.z + left.z * right.z},
                                 Vector3D<Value>{left.x * right.x + left.y * right.x + left.z * right.x,
                                                 left.x * right.y + left.y * right.y + left.z * right.y,
                                                 left.x * right.z + left.y * right.z + left.z * right.z}};
    }

    template <Arithmetic Value>
    [[nodiscard]] inline auto operator*(Value const factor,
                                        Rotation3D<Value> const& matrix) noexcept -> Rotation3D<Value>
    {
        return Rotation3D<Value>{matrix.x * factor, matrix.y * factor, matrix.z * factor};
    }

    template <Arithmetic Value>
    [[nodiscard]] inline auto operator*(Rotation3D<Value> const& matrix,
                                        Value const factor) noexcept -> Rotation3D<Value>
    {
        return Rotation3D<Value>{matrix.x * factor, matrix.y * factor, matrix.z * factor};
    }

    template <Arithmetic Value>
    [[nodiscard]] inline auto operator/(Rotation3D<Value> const& matrix,
                                        Value const factor) noexcept -> Rotation3D<Value>
    {
        return Rotation3D<Value>{matrix.x / factor, matrix.y / factor, matrix.z / factor};
    }

}; // namespace Linalg

#endif // ROTATION3D_HPP