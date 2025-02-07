#ifndef ROTATION3D_HPP
#define ROTATION3D_HPP

#include "vector3d.hpp"
#include <array>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <utility>

namespace Linalg {

    template <std::floating_point T>
    struct Rotation3D {
        [[nodiscard]] Rotation3D& operator+=(this Rotation3D& self, Rotation3D const& other) noexcept
        {
            self.x += other.x;
            self.y += other.y;
            self.z += other.z;
            return self;
        }

        [[nodiscard]] Rotation3D& operator-=(this Rotation3D& self, Rotation3D const& other) noexcept
        {
            self.x -= other.x;
            self.y -= other.y;
            self.z -= other.z;
            return self;
        }

        [[nodiscard]] Rotation3D& operator*=(this Rotation3D& self, Rotation3D const& other) noexcept
        {
            self.x = Vector3D<T>{self.x * other.x + self.y * other.x + self.z * other.x,
                                 self.x * other.y + self.y * other.y + self.z * other.y,
                                 self.x * other.z + self.y * other.z + self.z * other.z};
            self.y = Vector3D<T>{self.x * other.x + self.y * other.x + self.z * other.x,
                                 self.x * other.y + self.y * other.y + self.z * other.y,
                                 self.x * other.z + self.y * other.z + self.z * other.z};
            self.z = Vector3D<T>{self.x * other.x + self.y * other.x + self.z * other.x,
                                 self.x * other.y + self.y * other.y + self.z * other.y,
                                 self.x * other.z + self.y * other.z + self.z * other.z};
            return self;
        }

        [[nodiscard]] Rotation3D& operator*=(this Rotation3D& self, T const factor) noexcept
        {
            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        [[nodiscard]] Rotation3D& operator/=(this Rotation3D& self, T const factor) noexcept
        {
            self.x /= factor;
            self.y /= factor;
            self.z /= factor;
            return self;
        }

        [[nodiscard]] bool operator<=>(this Rotation3D const& self, Rotation3D const& other) noexcept = default;

        Vector3D<T> x{};
        Vector3D<T> y{};
        Vector3D<T> z{};
    };

    template <std::floating_point T>
    [[nodiscard]] Rotation3D<T> operator+(Rotation3D<T> const& left, Rotation3D<T> const& right) noexcept
    {
        return Rotation3D<T>{left.x + right.x, left.y + right.y, left.z + right.z};
    }

    template <std::floating_point T>
    [[nodiscard]] Rotation3D<T> operator-(Rotation3D<T> const& left, Rotation3D<T> const& right) noexcept
    {
        return Rotation3D<T>{left.x - right.x, left.y - right.y, left.z - right.z};
    }

    template <std::floating_point T>
    [[nodiscard]] Rotation3D<T> operator*(Rotation3D<T> const& left, Rotation3D<T> const& right) noexcept
    {
        return Rotation3D<T>{Vector3D<T>{left.x * right.x + left.y * right.x + left.z * right.x,
                                         left.x * right.y + left.y * right.y + left.z * right.y,
                                         left.x * right.z + left.y * right.z + left.z * right.z},
                             Vector3D<T>{left.x * right.x + left.y * right.x + left.z * right.x,
                                         left.x * right.y + left.y * right.y + left.z * right.y,
                                         left.x * right.z + left.y * right.z + left.z * right.z},
                             Vector3D<T>{left.x * right.x + left.y * right.x + left.z * right.x,
                                         left.x * right.y + left.y * right.y + left.z * right.y,
                                         left.x * right.z + left.y * right.z + left.z * right.z}};
    }

    template <std::floating_point T>
    [[nodiscard]] Rotation3D<T> operator*(T const factor, Rotation3D<T> const& matrix) noexcept
    {
        return Rotation3D<T>{matrix.x * factor, matrix.y * factor, matrix.z * factor};
    }

    template <std::floating_point T>
    [[nodiscard]] Rotation3D<T> operator*(Rotation3D<T> const& matrix, T const factor) noexcept
    {
        return Rotation3D<T>{matrix.x * factor, matrix.y * factor, matrix.z * factor};
    }

    template <std::floating_point T>
    [[nodiscard]] Rotation3D<T> operator/(Rotation3D<T> const& matrix, T const factor) noexcept
    {
        return Rotation3D<T>{matrix.x / factor, matrix.y / factor, matrix.z / factor};
    }

}; // namespace Linalg

#endif // ROTATION3D_HPP