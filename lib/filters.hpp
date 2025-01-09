#ifndef FILTERS_HPP
#define FILTERS_HPP

#include "common.hpp"
#include <functional>
#include <queue>
#include <utility>

namespace Linalg {
    template <Arithmetic Value>
    using Filter = std::function<Value(Value)>;

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto make_recursive_average(Value const start_condition = 0) noexcept -> auto
    {
        return [estimate = start_condition, samples = 1UL](Value const measurement) mutable {
            estimate = (estimate * (samples - 1UL) + measurement) / samples;
            samples += 1UL;
            return estimate;
        };
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto make_moving_average(Value const start_condition = 0,
                                                     Size const last_samples = 10UL) -> auto
    {
        assert(last_samples > 0);
        std::queue<Value> measurements{};
        for (auto i{0UL}; i < last_samples; ++i) {
            measurements.push(start_condition);
        }
        return [estimate = start_condition, last_samples, measurements = std::move(measurements)](
                   Value const measurement) mutable {
            estimate = estimate + (measurement - measurements.front()) / last_samples;
            if (!measurements.empty()) {
                measurements.pop();
            }
            measurements.push(measurement);
            return estimate;
        };
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto make_low_pass(Value const start_condition = 0,
                                               Value const alpha = 0.5) noexcept -> auto
    {
        assert(alpha >= 0 && alpha <= 1);
        return [estimate = start_condition, alpha](Value const measurement) mutable {
            estimate = (estimate * alpha) + (measurement * (Value{1} - alpha));
            return estimate;
        };
    }

}; // namespace Linalg

#endif // FILTERS_HPP