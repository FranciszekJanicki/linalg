#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <algorithm>
#include <complex>
#include <concepts>
#include <print>
#include <ranges>
#include <utility>

namespace Linalg {

    template <std::floating_point T, std::uint32_t N>
    using Polynomial = std::array<T, N + 1>;

    template <std::floating_point T>
    using Root = std::complex<T>;

    template <std::floating_point T, std::uint32_t N>
    using Roots = std::array<Root<T>, N>;

    template <std::floating_point T>
    [[nodiscard]] T delta(Polynomial<T, 2UL> const& polynomial) noexcept
    {
        auto const& [a, b, c]{polynomial};
        return std::pow(b, 2.0F) - 4.0F * a * c;
    }

    template <std::floating_point T>
    [[nodiscard]] Roots<T, 2UL> roots(Polynomial<T, 2UL> const& polynomial) noexcept
    {
        using Root = Root<T>;
        using Roots = Roots<T, 2UL>;

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

    template <std::floating_point T>
    [[nodiscard]] T delta(Polynomial<T, 3UL> const& polynomial) noexcept
    {
        auto const& [a, b, c, d]{polynomial};
        return std::pow(b, 2) * std::pow(c, 2) - 4.0F * a * std::pow(c, 3) - 4.0F * std::pow(b, 3) * d -
               27.0F * std::pow(a, 2) * std::pow(d, 2) + 18.0F * a * b * c * d;
    }

    template <std::floating_point T>
    [[nodiscard]] T delta(Polynomial<T, 4UL> const& polynomial) noexcept
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

    template <std::floating_point T, std::uint32_t N>
    [[nodiscard]] Polynomial<T, N> normalized(Polynomial<T, N> const& polynomial) noexcept
    {
        Polynomial<T, N> result{};
        std::ranges::transform(polynomial, result.data(), [first_coeff = polynomial.front()](T const coeff) {
            return coeff / first_coeff;
        });
        return result;
    }

    template <std::floating_point T, std::uint32_t N>
    [[nodiscard]] Polynomial<T, N - 1> derivative(Polynomial<T, N> const& polynomial) noexcept
    {
        Polynomial<T, N - 1> result{};
        for (std::uint32_t i{}; i < N - 1; ++i) {
            result[i] = polynomial[i] * static_cast<T>(N - i);
        }
        return result;
    }

    template <std::floating_point T, std::uint32_t N>
    [[nodiscard]] Polynomial<T, N + 1> integral(Polynomial<T, N> const& polynomial, T const constant = 0.0) noexcept

    {
        Polynomial<T, N + 1> result{};
        for (std::uint32_t i{}; i < N; ++i) {
            result[i] = polynomial[i] / static_cast<T>(N - i + 1);
        }
        result.back() = constant;
        return result;
    }

    template <std::floating_point T, std::uint32_t N>
    void print(Polynomial<T, N> const& polynomial) noexcept
    {
        std::println("");
        for (auto const& [index, coeff] : std::views::enumerate(polynomial)) {
            std::println(" {} * x^{} ", coeff, static_cast<std::uint32_t>(N - index));
        }
        std::println("");
    }

    template <std::floating_point T, std::uint32_t N>
    void print_roots(Polynomial<T, N> const& polynomial) noexcept
    {
        std::println("");
        for (auto const root : roots(polynomial)) {
            std::println(" Real: {} Imag: {} ", root.real(), root.imag());
        }
        std::println("");
    }

}; // namespace Linalg

#endif // POLYNOMIAL_HPP