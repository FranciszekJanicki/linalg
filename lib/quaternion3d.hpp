#ifndef QUATERNION3D_HPP
#define QUATERNION3D_HPP

#include <cmath>
#include <compare>
#include <concepts>
#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace Linalg {

    template <std::floating_point Value>
    struct Quaternion3D {
        [[nodiscard]] inline auto conjugated(this Quaternion3D const& self) noexcept -> Quaternion3D
        {
            return Quaternion3D{self.w, -self.x, -self.y, -self.z};
        }

        inline auto conjugate(this Quaternion3D& self) noexcept -> void
        {
            self.x = -self.x;
            self.y = -self.y;
            self.z = -self.z;
        }

        [[nodiscard]] inline auto magnitude(this Quaternion3D const& self) noexcept -> Value
        {
            return std::sqrt(std::pow(self.w, 2) + std::pow(self.x, 2) + std::pow(self.y, 2) + std::pow(self.z, 2));
        }

        [[nodiscard]] inline auto normalized(this Quaternion3D const& self) noexcept -> Quaternion3D
        {
            const auto im{static_cast<Value>(1) / self.magnitude()};
            return Quaternion3D{self.w * im, self.x * im, self.y * im, self.z * im};
        }

        inline auto normalize(this Quaternion3D& self) noexcept -> void
        {
            const auto im{static_cast<Value>(1) / self.magnitude()};
            self *= im;
        }

        [[nodiscard]] inline auto operator+=(this Quaternion3D& self, Quaternion3D const& other) -> Quaternion3D&
        {
            self.w += other.w;
            self.x += other.x;
            self.y += other.y;
            self.z += other.z;
            return self;
        }

        [[nodiscard]] inline auto operator-=(this Quaternion3D& self, Quaternion3D const& other) -> Quaternion3D&
        {
            self.w -= other.w;
            self.x -= other.x;
            self.y -= other.y;
            self.z -= other.z;
            return self;
        }

        [[nodiscard]] inline auto operator*=(this Quaternion3D& self, Quaternion3D const& other) -> Quaternion3D&
        {
            self.w = self.w * other.w - self.x * other.x - self.y * other.y - self.z * other.z;
            self.x = self.w * other.x + self.x * other.w + self.y * other.z - self.z * other.y;
            self.y = self.w * other.y - self.x * other.z + self.y * other.w + self.z * other.x;
            self.z = self.w * other.z + self.x * other.y - self.y * other.x + self.z * other.w;
            return self;
        }

        [[nodiscard]] inline auto operator*=(this Quaternion3D& self, Value const factor) -> Quaternion3D&
        {
            self.w *= factor;
            self.x *= factor;
            self.y *= factor;
            self.z *= factor;
            return self;
        }

        [[nodiscard]] inline auto operator/=(this Quaternion3D& self, Value const factor) -> Quaternion3D&
        {
            if (factor == Value{0.0}) {
                throw std::runtime_error{"Division by zero\n"};
            }

            self.w /= factor;
            self.x /= factor;
            self.y /= factor;
            self.z /= factor;
            return self;
        }

        template <std::floating_point Converted>
        [[nodiscard]] explicit inline operator Quaternion3D<Converted>(this Quaternion3D const& self) noexcept
        {
            return Quaternion3D<Converted>{static_cast<Converted>(self.w),
                                           static_cast<Converted>(self.x),
                                           static_cast<Converted>(self.y),
                                           static_cast<Converted>(self.z)};
        }

        [[nodiscard]] inline bool operator<=>(this Quaternion3D const& self,
                                              Quaternion3D const& other) noexcept = default;

        Value w{};
        Value x{};
        Value y{};
        Value z{};
    };

    template <std::floating_point Value>
    [[nodiscard]] inline auto operator+(Quaternion3D<Value> const& left, Quaternion3D<Value> const& right) noexcept
    {
        return Quaternion3D<Value>{left.w + right.w, left.x + right.x, left.y + right.y, left.z + right.z};
    }

    template <std::floating_point Value>
    [[nodiscard]] inline auto operator-(Quaternion3D<Value> const& left, Quaternion3D<Value> const& right) noexcept
    {
        return Quaternion3D<Value>{left.w - right.w, left.x - right.x, left.y - right.y, left.z + right.z};
    }

    template <std::floating_point Value>
    [[nodiscard]] inline auto operator*(Quaternion3D<Value> const& left, Quaternion3D<Value> const& right) noexcept
    {
        return Quaternion3D<Value>{left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z,
                                   left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y,
                                   left.w * right.y - left.x * right.z + left.y * right.w + left.z * right.x,
                                   left.w * right.z + left.x * right.y - left.y * right.x + left.z * right.w};
    }

    template <std::floating_point Value>
    [[nodiscard]] inline auto operator*(Quaternion3D<Value> const& quaternion, Value const factor)
    {
        return Quaternion3D<Value>{quaternion.w * factor,
                                   quaternion.x * factor,
                                   quaternion.y * factor,
                                   quaternion.z * factor};
    }

    template <std::floating_point Value>
    [[nodiscard]] inline auto operator*(Value const factor, Quaternion3D<Value> const& quaternion)
    {
        return quaternion * factor;
    }

    template <std::floating_point Value>
    [[nodiscard]] inline auto operator/(Quaternion3D<Value> const& quaternion, Value const factor)
    {
        if (factor == Value{0.0}) {
            throw std::runtime_error{"Disivion by zero\n"};
        }

        return Quaternion3D<Value>{quaternion.w / factor,
                                   quaternion.x / factor,
                                   quaternion.y / factor,
                                   quaternion.z / factor};
    }

}; // namespace Linalg

#endif // QUATERNION3D_HPP