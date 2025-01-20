#ifndef KALMAN_HPP
#define KALMAN_HPP

#include "common.hpp"
#include "stack_matrix.hpp"
#include <fmt/core.h>
#include <stdexcept>
#include <utility>

namespace Linalg::Observers {

    namespace {
        using namespace Stack;
    };

    template <Arithmetic Value, Size STATES, Size CONTROLS = 1UL, Size MEASUREMENTS = 1UL>
    struct Kalman {
        template <Size ROWS, Size COLS>
        using Matrix = Matrix<Value, ROWS, COLS>;

        [[nodiscard]] constexpr auto operator()(this Kalman& self,
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

        constexpr auto predict(this Kalman& self, Matrix<1UL, CONTROLS> const& control) -> void
        {
            try {
                self.state = (self.state_transition * self.state) + (self.control_transition * control);
                self.state_covariance =
                    (self.state_transition * self.state_covariance * matrix_transpose(self.state_transition)) +
                    self.process_noise;
            } catch (Error const& error) {
                throw error;
            }
        }

        constexpr auto correct(this Kalman& self, Matrix<1UL, MEASUREMENTS> const& measurement) -> void
        {
            try {
                auto const innovation{measurement - (self.measurement_transition * self.state)};
                auto const residual_covariance{(self.measurement_transition * self.state_covariance *
                                                matrix_transpose(self.measurement_transition)) +
                                               self.measurement_noise};
                auto const kalman_gain{self.state_covariance * matrix_transpose(self.measurement_transition) *
                                       matrix_inverse(residual_covariance)};
                self.state = self.state + (kalman_gain * innovation);
                self.state_covariance =
                    (make_eye<Value, STATES>() - kalman_gain * self.measurement_transition) * self.state_covariance;
            } catch (Error const& error) {
                throw error;
            }
        }

        Matrix<STATES, 1UL> state{};
        Matrix<STATES, STATES> state_covariance{};

        Matrix<STATES, STATES> state_transition{};
        Matrix<STATES, CONTROLS> control_transition{};
        Matrix<MEASUREMENTS, STATES> measurement_transition{};

        Matrix<MEASUREMENTS, MEASUREMENTS> measurement_noise{};
        Matrix<STATES, STATES> process_noise{};
    };

}; // namespace Linalg::Observers

#endif // KALMAN_HPP