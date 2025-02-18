#ifndef SIGNALS_HPP
#define SIGNALS_HPP

#include "utility.hpp"
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
#include <vector>

namespace Linalg::Signals {

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

    template <std::unsigned_integral Time, std::floating_point Value>
    struct Point {
        Time time_ms{};
        Value value{};
    };

    template <std::unsigned_integral Time, std::floating_point Value>
    [[nodiscard]] inline Value interpolate_point_1D(Point<Time, Value> const& point,
                                                    Point<Time, Value> const& next_point,
                                                    Time const interpolation_time)
    {
        auto& [next_time, next_value]{next_point};
        auto& [time, value]{point};

        if (next_time > static_cast<Time>(0) && next_time < time) {
            throw std::runtime_error{"next time smaller than time"};
        }
        if (interpolation_time > next_time || interpolation_time < time) {
            throw std::runtime_error{"Interpolation time outside of time and next time "};
        }

        Value slope{(next_value - value) / static_cast<Value>(next_time - time)};
        Value intercept{value - slope * static_cast<Value>(time)};
        return slope * static_cast<Value>(interpolation_time) + intercept;
    }

    template <std::unsigned_integral Time, std::floating_point Value>
    [[nodiscard]] inline std::vector<Value> interpolate_points_1D(std::vector<Point<Time, Value>> const& points,
                                                                  Time const sampling_time_ms)
    {
        auto i = 0UL;
        auto current_time = points.front().time_ms;
        auto total_points = (points.back().time_ms - points.front().time_ms) / sampling_time_ms;
        auto interpolated_points = std::vector<Value>{};
        interpolated_points.reserve(total_points);

        while (current_time < points.back().time_ms && i < total_points) {
            try {
                interpolated_points.push_back(
                    interpolate_point_1D(points[i],
                                         points[i + 1UL],
                                         std::exchange(current_time, current_time + sampling_time_ms)));

                if (current_time >= points[i + 1UL].time_ms) {
                    ++i;
                }
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        return interpolated_points;
    }

    template <std::unsigned_integral Time, std::floating_point Value>
    [[nodiscard]] inline auto make_interpolator_1D(std::vector<Point<Time, Value>> const& points,
                                                   Time const sampling_time_ms)
    {
        return [points = interpolate_points_1D(points, sampling_time_ms), i = 0UL]() mutable {
            if (i >= points.size()) {
                throw std::runtime_error{"End of points"};
            }

            return points[++i];
        };
    }

    template <std::unsigned_integral Time, std::floating_point Value>
    [[nodiscard]] inline auto make_interpolator_1D(std::vector<Point<Time, Value>> const& points)
    {
        return [points, i = 0UL, current_time = points.front().time_ms](Time const sampling_time_ms) mutable {
            if (i >= points.size() || current_time > points.back().time_ms) {
                throw std::runtime_error{"End of points"};
            }
            try {
                auto current_interpolation =
                    interpolate_point_1D(points[i],
                                         points[i + 1UL],
                                         std::exchange(current_time, current_time + sampling_time_ms));

                if (current_time >= points[i + 1UL].time_ms) {
                    ++i;
                }

                return current_interpolation;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        };
    }

}; // namespace Linalg::Signals

#endif // SIGNALS_HPP