#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <numeric>
#include <ranges>
#include <utility>

namespace Signals {

    template <std::floating_point T, std::size_t N, typename Formula>
    [[nodiscard]] std::array<T, N> make_values(Formula&& formula) noexcept
    {
        std::array<T, N> values{};
        for (auto& [time, value] : std::views::enumerate(values)) {
            value = std::invoke(std::forward<Formula>(formula), time);
        }
        return values;
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] std::array<T, N> make_tangens(T const amplitude, std::uint32_t const frequency) noexcept

    {
        return make_values([amplitude, omega = static_cast<T>(2.0) * static_cast<T>(M_PI) * static_cast<T>(frequency)](
                               auto const time) { return amplitude * std::tan(omega * static_cast<T>(time)); });
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] std::array<T, N> make_sine(T const amplitude, std::uint32_t const frequency) noexcept

    {
        return make_values([amplitude, omega = static_cast<T>(2.0) * static_cast<T>(M_PI) * static_cast<T>(frequency)](
                               auto const time) { return amplitude * std::sin(omega * static_cast<T>(time)); });
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] std::array<T, N> make_cosine(T const amplitude, std::uint32_t const frequency) noexcept

    {
        return make_values([amplitude, omega = static_cast<T>(2.0) * static_cast<T>(M_PI) * static_cast<T>(frequency)](
                               auto const time) { return amplitude * std::cos(omega * static_cast<T>(time)); });
    }

}; // namespace Signals

#endif // SIGNALS_HPP