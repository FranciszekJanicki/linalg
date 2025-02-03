#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include "common.hpp"
#include <algorithm>
#include <complex>
#include <ranges>
#include <utility>

namespace Linalg {

    template <Arithmetic Value, Size ORDER>
    using Polynomial = std::array<Value, ORDER + 1>;

    template <Arithmetic Value>
    using Root = std::complex<Value>;

    template <Arithmetic Value, Size ORDER>
    using Roots = std::array<Root<Value>, ORDER>;

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto delta(Polynomial<Value, 2UL> const& polynomial) noexcept -> Value
    {
        auto const& [a, b, c]{polynomial};
        return std::pow(b, 2.0F) - 4.0F * a * c;
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto roots(Polynomial<Value, 2UL> const& polynomial) noexcept -> Roots<Value, 2UL>
    {
        using Root = Root<Value>;
        using Roots = Roots<Value, 2UL>;

        auto const& [a, b, c]{polynomial};
        auto const delta_val{delta(polynomial)};
        if (delta_val > 0) {
            return Roots{Root{(-b + std::sqrt(delta_val)) / (2.0F * a), 0.0F},
                         Root{(-b - std::sqrt(delta_val)) / (2.0F * a), 0.0F}};
        } else if (delta_val < 0) {
            return Roots{Root{-b / (2.0F * a), std::sqrt(-delta_val) / (2.0F * a)},
                         Root{-b / (2.0F * a), -std::sqrt(-delta_val) / (2.0F * a)}};
        } else {
            return Roots{Root{-b / (2.0F * a), 0.0F}, Root{-b / (2.0F * a), 0.0F}};
        }
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto delta(Polynomial<Value, 3UL> const& polynomial) noexcept -> Value
    {
        auto const& [a, b, c, d]{polynomial};
        return std::pow(b, 2) * std::pow(c, 2) - 4.0F * a * std::pow(c, 3) - 4.0F * std::pow(b, 3) * d -
               27.0F * std::pow(a, 2) * std::pow(d, 2) + 18.0F * a * b * c * d;
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto delta(Polynomial<Value, 4UL> const& polynomial) noexcept -> Value
    {
        auto const& [a, b, c, d, e]{polynomial};
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

    template <Arithmetic Value, Size ORDER>
    [[nodiscard]] constexpr auto normalized(Polynomial<Value, ORDER> const& polynomial) noexcept
        -> Polynomial<Value, ORDER>
    {
        Polynomial<Value, ORDER> result{};
        std::ranges::transform(polynomial, result.data(), [first_coeff = polynomial.front()](Value const coeff) {
            return coeff / first_coeff;
        });
        return result;
    }

    template <Arithmetic Value, Size ORDER>
    [[nodiscard]] constexpr auto derivative(Polynomial<Value, ORDER> const& polynomial) noexcept
        -> Polynomial<Value, ORDER - 1>
    {
        Polynomial<Value, ORDER - 1> result{};
        for (Size i{}; i < ORDER - 1; ++i) {
            result[i] = polynomial[i] * (ORDER - i);
        }
        return result;
    }

    template <Arithmetic Value, Size ORDER>
    [[nodiscard]] constexpr auto integral(Polynomial<Value, ORDER> const& polynomial,
                                          Value const constant = 0.0) noexcept -> Polynomial<Value, ORDER + 1>
    {
        Polynomial<Value, ORDER + 1> result{};
        for (Size i{}; i < ORDER; ++i) {
            result[i] = polynomial[i] / (ORDER - i + 1);
        }
        result.back() = constant;
        return result;
    }

    template <Arithmetic Value, Size ORDER>
    constexpr auto print(Polynomial<Value, ORDER> const& polynomial) noexcept -> void
    {
        fmt::println("");
        for (auto const& [index, coeff] : std::views::enumerate(polynomial)) {
            fmt::println(" {} * x^{} ", coeff, ORDER - index);
        }
        fmt::println("");
    }

    template <Arithmetic Value, Size ORDER>
    constexpr auto print_roots(Polynomial<Value, ORDER> const& polynomial) noexcept -> void
    {
        fmt::println("");
        for (auto const root : roots(polynomial)) {
            fmt::println(" Real: {} Imag: {} ", root.real(), root.imag());
        }
        fmt::println("");
    }

    template <Arithmetic Value, Size ORDER>
    constexpr auto print_derivative(Polynomial<Value, ORDER> const& polynomial) noexcept -> void
    {
        print<Value, ORDER - 1>(derivative<Value, ORDER>(polynomial));
    }

    template <Arithmetic Value, Size ORDER>
    constexpr auto print_integral(Polynomial<Value, ORDER> const& polynomial) noexcept -> void
    {
        print<Value, ORDER + 1>(integral<Value, ORDER>(polynomial));
    }
    template <Arithmetic Value, Size ORDER>
    constexpr auto print_normalized(Polynomial<Value, ORDER> const& polynomial) noexcept -> void
    {
        print<Value, ORDER>(normalized<Value, ORDER>(polynomial));
    }

}; // namespace Linalg

#endif // POLYNOMIAL_HPP