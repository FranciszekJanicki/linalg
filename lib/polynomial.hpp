#ifndef POLYNOMIAL
#define POLYNOMIAL

#include "common.hpp"
#include <complex>
#include <utility>

namespace Linalg {

    template <Arithmetic Value, Size ORDER>
    struct Polynomial {
    public:
        using Root = std::complex<Value>;
        using Roots = std::array<Root, 2UL>;
        using Coeffs = std::array<Value, 3UL>;

        constexpr explicit Polynomial(Coeffs&& coeffs) noexcept : coeffs_{std::forward<Coeffs>(coeffs)}
        {}

        constexpr explicit Polynomial(Coeffs const& coeffs) noexcept : coeffs_{coeffs}
        {}

    private:
        Coeffs coeffs_;
    };

    template <Arithmetic Value>
    struct Polynomial<Value, 2UL> {
    public:
        using Root = std::complex<Value>;
        using Roots = std::array<Root, 2UL>;
        using Coeffs = std::array<Value, 3UL>;

        constexpr explicit Polynomial(Coeffs&& coeffs) noexcept : coeffs_{std::forward<Coeffs>(coeffs)}
        {}

        constexpr explicit Polynomial(Coeffs const& coeffs) noexcept : coeffs_{coeffs}
        {}

        [[nodiscard]] constexpr auto delta(this Polynomial const& self) noexcept -> Value
        {
            auto const& [a, b, c]{self.coeffs_};
            return std::pow(b, 2) - 4.0F * a * c;
        }

        [[nodiscard]] constexpr auto roots(this Polynomial const& self) noexcept -> Roots
        {
            auto const& [a, b, c]{self.coeffs_};
            auto const delta{self.delta()};
            if (delta > 0) {
                return Roots{Root{(-b + std::sqrt(delta)) / (2.0F * a), 0.0F},
                             Root{(-b - std::sqrt(delta)) / (2.0F * a), 0.0F}};
            } else if (delta < 0) {
                return Roots{Root{-b / (2.0F * a), std::sqrt(-delta) / (2.0F * a)},
                             Root{-b / (2.0F * a), -std::sqrt(-delta) / (2.0F * a)}};
            } else {
                return Roots{Root{-b / (2.0F * a), 0.0F}, Root{-b / (2.0F * a), 0.0F}};
            }
        }

    private:
        Coeffs coeffs_;
    };

    template <Arithmetic Value>
    struct Polynomial<Value, 3UL> {
    public:
        using Root = std::complex<Value>;
        using Roots = std::array<Root, 3UL>;
        using Coeffs = std::array<Value, 4UL>;

        constexpr explicit Polynomial(Coeffs&& coeffs) noexcept : coeffs_{std::forward<Coeffs>(coeffs)}
        {}

        constexpr explicit Polynomial(Coeffs const& coeffs) noexcept : coeffs_{coeffs}
        {}

        [[nodiscard]] constexpr auto delta(this Polynomial const& self) noexcept -> Value
        {
            auto const& [a, b, c, d]{self.coeffs_};
            return std::pow(b, 2) * std::pow(c, 2) - 4.0F * a * std::pow(c, 3) - 4.0F * std::pow(b, 3) * d -
                   27.0F * std::pow(a, 2) * std::pow(d, 2) + 18.0F * a * b * c * d;
        }

        [[nodiscard]] constexpr auto roots(this Polynomial const& self) noexcept -> Roots
        {
            auto const& [a, b, c, d]{self.coeffs_};
            auto const delta{self.delta()};
            if (delta > 0) {
                return Roots{};
            } else if (delta < 0) {
                return Roots{};
            } else {
                return Roots{};
            }
        }

    private:
        Coeffs coeffs_;
    };

    template <Arithmetic Value>
    struct Polynomial<Value, 4UL> {
    public:
        using Root = std::complex<Value>;
        using Roots = std::array<Root, 4UL>;
        using Coeffs = std::array<Value, 5UL>;

        constexpr explicit Polynomial(Coeffs&& coeffs) noexcept : coeffs_{std::forward<Coeffs>(coeffs)}
        {}

        constexpr explicit Polynomial(Coeffs const& coeffs) noexcept : coeffs_{coeffs}
        {}

        [[nodiscard]] constexpr auto delta(this Polynomial const& self) noexcept -> Value
        {
            auto const& [a, b, c, d, e]{self.coeffs_};
            return 256.0F * std::pow(a, 3) * std::pow(e, 3) - 192.0F * std::pow(a, 2) * b * d * std::pow(e, 2) -
                   128.0F * std::pow(a, 2) * std::pow(c, 2) * std::pow(e, 2) +
                   144.0F * std::pow(a, 2) * c * std::pow(d, 2) * e - 27.0F * std::pow(a, 2) * std::pow(d, 4) +
                   144.0F * a * std::pow(b, 2) * c * std::pow(e, 2) - 6.0F * a * std::pow(b, 2) * std::pow(d, 2) * e -
                   80.0F * a * b * std::pow(c, 2) * d * e + 18.0F * a * b * c * std::pow(d, 3) +
                   16.0F * a * std::pow(c, 4) * e - 4.0F * a * std::pow(c, 3) * std::pow(d, 2) -
                   27.0F * std::pow(b, 4) * std::pow(e, 2) + 18.0F * std::pow(b, 3) * c * d * e -
                   4.0F * std::pow(b, 3) * std::pow(d, 3) - 4.0F * std::pow(b, 2) * std::pow(c, 3) * e +
                   std::pow(b, 2) * std::pow(c, 2) * std::pow(d, 2);
        }

        [[nodiscard]] constexpr auto roots(this Polynomial const& self) noexcept -> Roots
        {
            auto const& [a, b, c, d]{self.coeffs_};
            auto const delta{self.delta()};
            if (delta > 0) {
                return Roots{};
            } else if (delta < 0) {
                return Roots{};
            } else {
                return Roots{};
            }
        }

    private:
        Coeffs coeffs_;
    };

}; // namespace Linalg

#endif // POLYNOMIAL