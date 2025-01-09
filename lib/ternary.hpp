#ifndef TERNARY_HPP
#define TERNARY_HPP

#include "common.hpp"

namespace Regulators {

    template <Linalg::Arithmetic Value>
    struct Ternary {
        enum struct State {
            POSITIVE,
            NEGATIVE,
            ZERO,
        };

        [[nodiscard]]
        inline auto operator()(this Ternary& self, const Value error) noexcept -> State
        {
            switch (self.state) {
                case State::POSITIVE:
                    if (error < self.hysteresis_down) {
                        self.state = State::ZERO;
                    }
                    break;
                case State::NEGATIVE:
                    if (error > self.hysteresis_up) {
                        self.state = State::ZERO;
                    }
                    break;
                case State::ZERO:
                    if (error > self.hysteresis_up) {
                        self.state = State::POSITIVE;
                    } else if (error < self.hysteresis_down) {
                        self.state = State::NEGATIVE;
                    }
                    break;
                default:
                    break;
            }
            return self.state;
        }

        Value hysteresis_up{};
        Value hysteresis_down{};

        State state{State::ZERO};
    };

}; // namespace Regulators

#endif // TERNARY_HPP