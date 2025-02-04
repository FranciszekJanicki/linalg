#ifndef SIGSAMPLESALS_HPP
#define SIGSAMPLESALS_HPP

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

    template <std::floating_point Value, std::size_t SAMPLES, typename Formula>
    [[nodiscard]] constexpr auto make_values(Formula&& formula) noexcept -> std::array<Value, SAMPLES>
    {
        std::array<Value, SAMPLES> values{};
        for (auto& [time, value] : std::views::enumerate(values)) {
            value = std::invoke(std::forward<Formula>(formula), time);
        }
        return values;
    }

    template <std::floating_point Value, std::size_t SAMPLES>
    [[nodiscard]] constexpr auto make_tangens(Value const amplitude, std::uint32_t const frequency) noexcept
        -> std::array<Value, SAMPLES>
    {
        return make_values(
            [amplitude, omega = static_cast<Value>(2.0) * static_cast<Value>(M_PI) * static_cast<Value>(frequency)](
                auto const time) { return amplitude * std::tan(omega * static_cast<Value>(time)); });
    }


    template <std::floating_point Value, std::size_t SAMPLES>
    [[nodiscard]] constexpr auto make_sine(Value const amplitude, std::uint32_t const frequency) noexcept
        -> std::array<Value, SAMPLES>
    {
        return make_values(
            [amplitude, omega = static_cast<Value>(2.0) * static_cast<Value>(M_PI) * static_cast<Value>(frequency)](
                auto const time) { return amplitude * std::sin(omega * static_cast<Value>(time)); });
    }

    template <std::floating_point Value, std::size_t SAMPLES>
    [[nodiscard]] constexpr auto make_cosine(Value const amplitude, std::uint32_t const frequency) noexcept
        -> std::array<Value, SAMPLES>
    {
        return make_values(
            [amplitude, omega = static_cast<Value>(2.0) * static_cast<Value>(M_PI) * static_cast<Value>(frequency)](
                auto const time) { return amplitude * std::cos(omega * static_cast<Value>(time)); });
    }

}; // namespace Signals

#endif // SIGSAMPLESALS_HPP