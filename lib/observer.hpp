#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "common.hpp"
#include "stack_matrix.hpp"

namespace Linalg::Observers {

    template <Arithmetic Value, Size STATES, Size CONTROLS = 1UL, Size MEASUREMENTS = 1UL>
    struct Observer {
    public:
        template <Size ROWS, Size COLS>
        using Matrix = Stack::Matrix<Value, ROWS, COLS>;

        [[nodiscard]] constexpr auto operator()(this Observer& self,
                                                Matrix<1UL, CONTROLS> const& control,
                                                Matrix<1UL, MEASUREMENTS> const& measurement) -> Matrix<STATES, 1UL>
        {
            try {
                self.predict(control);
                self.correct(measurement);
                return self.state;
            } catch (Error const& error) {
                throw error;
            }
        }

        constexpr auto predict(this Observer& self, Matrix<1UL, CONTROLS> const& control) -> Matrix<STATES, 1UL>
        {
            try {
                self.state = self.state_transition * self.state + self.control_transition * control;
                return self.state;
            } catch (Error const& error) {
                throw error;
            }
        }

        constexpr auto predict(this Observer& self, Matrix<1UL, MEASUREMENTS> const& measurement) -> Matrix<STATES, 1UL>
        {
            try {
                self.state = self.state + self.state_gain * (measurement - self.measurement_transition * self.state);
                return self.state;
            } catch (Error const& error) {
                throw error;
            }
        }

        Matrix<STATES, 1UL> state{};
        Matrix<STATES, 1UL> state_gain{};
        Matrix<STATES, STATES> state_transition{};
        Matrix<STATES, CONTROLS> control_transition{};
        Matrix<MEASUREMENTS, 1UL> measurement_transition{};
    };

}; // namespace Linalg::Observers

#endif // OBSERVER_HPP