#ifndef TERNARY_HPP
#define TERNARY_HPP

#include <concepts>

namespace Linalg::Regulators {

    template <std::floating_point Value>
    struct Ternary {
        enum struct State {
            POSITIVE,
            NEGATIVE,
            ZERO,
        };

        [[nodiscard]] auto operator()(this Ternary& self,  Value const error) noexcept -> State
        {
            return self.state = self.get_state(error);
        }

        auto get_state(this Ternary const& self, Value const error) noexcept ->State {
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

        Value hysteresis_up{};
        Value hysteresis_down{};

        State state{State::ZERO};
    };

}; // namespace Linalg::Regulators

#endif // TERNARY_HPP