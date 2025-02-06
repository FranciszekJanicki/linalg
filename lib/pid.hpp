#ifndef PID_HPP
#define PID_HPP

#include <algorithm>
#include <concepts>
#include <numeric>
#include <stdexcept>
#include <utility>

namespace Linalg::Regulators {

    template <std::floating_point Value>
    [[nodiscard]] auto differentiate(Value const value,
                                  Value const prev_value,
                                  Value const sampling_time,
                                  Value const prev_derivative,
                                  Value const time_constant) -> Value
    {
        if (time_constant + sampling_time == static_cast<Value>(0.0F)) {
            throw std::runtime_error{"Division by 0!"};
        }
        return (value - prev_value + prev_derivative * time_constant) / (time_constant + sampling_time);
    }

    template <std::floating_point Value>
    [[nodiscard]] auto differentiate(Value const value, Value const prev_value, Value const sampling_time) -> Value
    {
        if (sampling_time == static_cast<Value>(0.0F)) {
            throw std::runtime_error{"Division by 0!"};
        }
        return (value - prev_value) / sampling_time;
    }

    template <std::floating_point Value>
    [[nodiscard]] auto integrate(Value const value, Value const prev_value, Value const sampling_time) noexcept -> Value
    {
        return (value + prev_value) * static_cast<Value>(0.5F) * sampling_time;
    }

    template <std::floating_point Value>
    struct PID {
        [[nodiscard]] auto operator()(this PID& self, Value const error, Value const sampling_time) noexcept -> Value
        {
            return self.get_control(error, sampling_time);
        }

        auto get_control(this PID& self, Value const error, Value const sampling_time) noexcept -> Value
        {
            auto control{self.proportion_gain * error + self.integral_gain * self.get_integral(error, sampling_time) +
                         self.derivative_gain * self.get_derivative(error, sampling_time)};
            auto sat_control{std::clamp(control, -self.saturation, self.saturation)};
            self.prev_sat_error = control - sat_control;
            return sat_control;
        }

        auto get_derivative(this PID& self, Value const error, Value const sampling_time) noexcept -> Value
        {
            self.derivative =
                differentiate(error, std::exchange(self.prev_error, error), sampling_time, self.derivative, self.time_constant);
            return self.derivative;
        }

        auto get_integral(this PID& self, Value const error, Value const sampling_time) noexcept -> Value
        {
            auto integrated{self.integral_gain == Value{0} ? error : error - self.control_gain * self.prev_sat_error / self.integral_gain};
            self.integral += integrate(integrated, std::exchange(self.prev_integrated, integrated), sampling_time);
            return self.integral;
        }

        Value proportion_gain{};
        Value integral_gain{};
        Value derivative_gain{};
        Value control_gain{};
        Value saturation{};
        Value time_constant{};

        Value integral{0.0F};
        Value derivative{0.0F};
        Value prev_error{0.0F};
        Value prev_sat_error{0.0F};
        Value prev_integrated{0.0F};
    };

}; // namespace Linalg::Regulators

#endif // PID_HPP
