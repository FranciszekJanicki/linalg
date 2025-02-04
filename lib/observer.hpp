#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "stack_matrix.hpp"
#include <concepts>

namespace Linalg::Observers {

    template <std::floating_point Value, std::size_t STATES, std::size_t CONTROLS = 1UL, std::size_t MEASUREMENTS = 1UL>
    struct Observer {
    public:
        template <std::size_t ROWS, std::size_t COLS>
        using Matrix = Stack::Matrix<Value, ROWS, COLS>;

        [[nodiscard]] auto operator()(this Observer& self,
                                      Matrix<1UL, CONTROLS> const& control,
                                      Matrix<1UL, MEASUREMENTS> const& measurement) -> Matrix<STATES, 1UL>
        {
            try {
                self.predict(control);
                self.correct(measurement);
                return self.state;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        auto predict(this Observer& self, Matrix<1UL, CONTROLS> const& control) -> void
        {
            try {
                self.state = self.state_transition * self.state + self.control_transition * control;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        auto predict(this Observer& self, Matrix<1UL, MEASUREMENTS> const& measurement) -> void
        {
            try {
                self.state = self.state + self.state_gain * (measurement - self.measurement_transition * self.state);
            } catch (std::runtime_error const& error) {
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