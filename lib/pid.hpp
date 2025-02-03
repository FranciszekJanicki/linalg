#ifndef PID_HPP
#define PID_HPP

#include "common.hpp"
#include <algorithm>
#include <numeric>

namespace Linalg::Regulators {

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto derivative(Value const value, Value const prev_value, Value const dt) -> Value
    {
        if (dt == Value{0}) {
            throw Error{"Division by 0!"};
        }
        return (value - prev_value) / dt;
    }

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto integral(Value const value, Value const prev_value, Value const dt) noexcept -> Value
    {
        return (value + prev_value) / 2.0F * dt;
    }

    template <Arithmetic Value>
    struct PID {
        [[nodiscard]] constexpr auto operator()(this PID& self, Value const error, Value const dt) noexcept -> Value
        {
            return self.get_saturated_control(error, dt);
        }

        constexpr auto get_saturation_error(this PID& self) noexcept -> Value
        {
            return self.prev_control - self.saturate(self.prev_control);
        }

        constexpr auto get_integrated(this PID& self, Value const error) noexcept -> Value
        {
            if (self.ki == Value{0}) {
                return error;
            }
            return error - self.kc * self.get_saturation_error() / self.ki;
        }

        constexpr auto get_control(this PID& self, Value const error, Value const dt) noexcept -> Value
        {
            auto control{self.kp * error + self.ki * self.get_integral(error, dt) +
                         self.kd * self.get_derivative(error, dt)};
            return self.prev_control = control;
        }

        constexpr auto get_derivative(this PID& self, Value const error, Value const dt) noexcept -> Value
        {
            return derivative(error, std::exchange(self.prev_error, error), dt);
        }

        constexpr auto get_integral(this PID& self, Value const error, Value const dt) noexcept -> Value
        {
            auto integrated{self.get_integrated(error)};
            return self.integral_value += integral(integrated, std::exchange(self.prev_integrated, integrated), dt);
        }

        constexpr auto saturate(this PID& self, Value const control) noexcept -> Value
        {
            return std::clamp(control, -self.saturation, self.saturation);
        }

        constexpr auto get_saturated_control(this PID& self, Value const error, Value const dt) noexcept -> Value
        {
            return self.saturate(self.get_control(error, dt));
        }

        Value kp{};
        Value ki{};
        Value kd{};
        Value kc{};
        Value saturation{};

        Value integral_value{0.0F};
        Value prev_error{0.0F};
        Value prev_control{0.0F};
        Value prev_integrated{0.0F};
    };

}; // namespace Linalg::Regulators

#endif // PID_HPP
