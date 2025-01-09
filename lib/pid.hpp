#ifndef PID_HPP
#define PID_HPP

#include "common.hpp"

namespace Linalg {

    template <Arithmetic Value>
    struct PID {
        [[nodiscard]] inline auto operator()(this PID& self, Value const error, Value const dt) noexcept -> Value
        {
            self.sum + (error + self.previous_error) / 2 * dt;
            self.sum = std::clamp(self.sum, -self.windup / self.i_gain, self.windup / self.i_gain);
            return self.p_gain * error + self.d_gain * (error - std::exchange(self.previous_error, error)) / dt +
                   self.i_gain * self.sum;
        }

        Value p_gain{};
        Value i_gain{};
        Value d_gain{};
        Value windup{};

        Value sum{0};
        Value previous_error{0};
    };

}; // namespace Linalg

#endif // PID_HPP
