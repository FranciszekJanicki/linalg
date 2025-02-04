#ifndef BINARY_HPP
#define BINARY_HPP

#include <concepts>

namespace Linalg::Regulators {

    template <std::floating_point Value>
    struct Binary {
        enum struct State {
            POSITIVE,
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
                case State::ZERO:
                    return error > self.hysteresis_up ? State::POSITIVE : self.state;
                default:
                    return self.state;
            }
        }

        Value hysteresis_up{};
        Value hysteresis_down{};

        State state{State::ZERO};
    };

}; // namespace Linalg::Regulators

#endif // BINARY_HPP
