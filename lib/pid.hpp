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
            auto control{self.get_proportion(error) + self.get_integral(error, sampling_time) +
                         self.get_derivative(error, sampling_time)};
            auto saturated_control{std::clamp(control, -self.saturation, self.saturation)};
            self.prev_sat_error = std::exchange(self.sat_error, control - saturated_control);
            self.prev_error = error;
            return saturated_control;
        }

        auto get_proportion(this PID& self, Value const error) noexcept -> Value {
            return self.proportion_gain * error;
        }

        auto get_derivative(this PID& self, Value const error, Value const sampling_time) noexcept -> Value
        {
            self.error_derivative =
                differentiate(error, self.prev_error, sampling_time, self.error_derivative, self.time_constant);
            return self.derivative_gain * self.error_derivative;
        }

        auto get_integral(this PID& self, Value const error, Value const sampling_time) noexcept -> Value
        {
            self.error_integral += integrate(error, self.prev_error, sampling_time);
            self.sat_error_integral += integrate(self.sat_error, self.prev_sat_error, sampling_time);
            return self.integral_gain * self.error_integral - self.control_gain * self.sat_error_integral;
        }

        // basic PID parameters
        Value proportion_gain{};
        Value integral_gain{};
        Value derivative_gain{};
        Value time_constant{};

        // anti windup parameters
        Value control_gain{};
        Value saturation{};
       
        // basic PID internal state
        Value prev_error{0.0F};
        Value error_integral{0.0F};
        Value error_derivative{0.0F};

        // anti windup internal state
        Value sat_error{0.0F};
        Value prev_sat_error{0.0F};
        Value sat_error_integral{0.0F};
    };

}; // namespace Linalg::Regulators

#endif // PID_HPP
