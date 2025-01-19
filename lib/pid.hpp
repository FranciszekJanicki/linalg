#ifndef PID_HPP
#define PID_HPP

#include "common.hpp"
#include <algorithm>
#include <numeric>

namespace Linalg::Regulators {

    template <Arithmetic Value>
    [[nodiscard]] constexpr auto derivative(Value const value, Value const prev_value, Value const dt) noexcept -> Value
    {
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
            auto control{self.kp * error + self.ki * self.sum +
                         self.kd * derivative(error, std::exchange(self.prev_error, error), dt)};
            auto sat_control{std::clamp(control, -self.sat, self.sat)};
            auto windup{control - sat_control};
            self.sum += integral(windup, std::exchange(self.prev_windup, windup), dt);
            return sat_control;
        }

        Value kp{};
        Value ki{};
        Value kd{};
        Value sat{};

        Value sum{0.0F};
        Value prev_error{0.0F};
    };

}; // namespace Linalg::Regulators

#endif // PID_HPP
