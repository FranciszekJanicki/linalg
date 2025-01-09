#ifndef BINARY_HPP
#define BINARY_HPP

#include "arithmetic.hpp"

namespace Regulators {

    template <Linalg::Arithmetic Value>
    struct Binary {
        enum struct State {
            POSITIVE,
            ZERO,
        };

        [[nodiscard]] inline auto operator()(this Binary& self, const Value error) noexcept -> State
        {
            switch (self.state) {
                case State::POSITIVE:
                    if (error < self.hysteresis_down) {
                        self.state = State::ZERO;
                    }
                    break;
                case State::ZERO:
                    if (error > self.hysteresis_up) {
                        self.state = State::POSITIVE;
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

#endif // BINARY_HPP
