#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <complex>
#include <concepts>
#include <numeric>
#include <ranges>

namespace Linalg::Transform {

    template <std::floating_point T, std::size_t N>
    using DFT = std::array<std::complex<T>, N>;

    template <std::floating_point T, std::size_t N>
    using IDFT = std::array<T, N>;

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] DFT<T, N> dft(IDFT<T, N> const& idft) noexcept
    {
        DFT<T, N> dft{};
        for (auto [k, X] : std::views::enumerate(dft)) {
            X = 0.0F;
            for (auto [n, x] : std::views::enumerate(idft)) {
                auto const arg = static_cast<T>(2.0) * static_cast<T>(M_PI) * static_cast<T>(n * k) / static_cast<T>(N);
                X += x * std::complex{std::cos(arg), static_cast<T>(-1.0) * std::sin(arg)};
            }
        }
        return dft;
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] IDFT<T, N> idft(DFT<T, N> const& dft) noexcept
    {
        IDFT<T, N> idft{};
        for (auto [n, x] : std::views::enumerate(idft)) {
            x = 0.0F;
            for (auto [k, X] : std::views::enumerate(dft)) {
                auto const arg = static_cast<T>(2.0) * static_cast<T>(M_PI) * static_cast<T>(n * k) / static_cast<T>(N);
                x += X.real() * std::cos(arg) + X.imag() * std::sin(arg);
            }
            x /= N;
        }
        return idft;
    }

}; // namespace Linalg::Transform

#endif // TRANSFORMS_HPP