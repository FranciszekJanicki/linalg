#ifndef PID_HPP
#define PID_HPP

#include "common.hpp"

namespace Linalg {

    template <Arithmetic Value>
    struct PID {
        [[nodiscard]] inline auto operator()(this PID& self, Value const error, Value const dt) noexcept -> Value
        {
            self.sum + (error + self.previous_error) / 2.0F * dt;
            self.sum = std::clamp(self.sum, -self.windup / self.integral_gain, self.windup / self.integral_gain);
            return self.proportional_gain * error +
                   self.derivative_gain * (error - std::exchange(self.previous_error, error)) / dt +
                   self.integral_gain * self.sum;
        }

        Value proportional_gain{};
        Value integral_gain{};
        Value derivative_gain{};
        Value windup{};

        Value sum{0};
        Value previous_error{0};
    };

}; // namespace Linalg

#endif // PID_HPP
