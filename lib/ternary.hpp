#ifndef TERNARY_HPP
#define TERNARY_HPP

#include <concepts>

namespace Linalg::Regulators {

    template <std::floating_point T>
    struct Ternary {
        enum struct State {
            POSITIVE,
            NEGATIVE,
            ZERO,
        };

        [[nodiscard]] State operator()(this Ternary& self, T const error) noexcept
        {
            return self.state = self.get_state(error);
        }

        State get_state(this Ternary const& self, T const error) noexcept
        {
            switch (self.state) {
                case State::POSITIVE:
                    return error < self.hysteresis_down ? State::ZERO : self.state;
                case State::NEGATIVE:
                    return error > self.hysteresis_up ? State::ZERO : self.state;
                case State::ZERO:
                    return error > self.hysteresis_up ? State::POSITIVE : self.state;
                    return error < self.hysteresis_down ? State::NEGATIVE : self.state;
                default:
                    return self.state;
            }
        }

        T hysteresis_up{};
        T hysteresis_down{};

        State state{State::ZERO};
    };

}; // namespace Linalg::Regulators

#endif // TERNARY_HPP