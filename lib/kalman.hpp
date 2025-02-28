#ifndef KALMAN_HPP
#define KALMAN_HPP

#include "common.hpp"
#include "stack_matrix.hpp"
#include <fmt/core.h>
#include <stdexcept>
#include <utility>

namespace Linalg {

    namespace {
        using namespace Stack;
    };

    template <Arithmetic Value, Size STATES, Size INPUTS = 1UL, Size MEASUREMENTS = 1UL>
    struct Kalman {
        template <Size ROWS, Size COLS>
        using Matrix = Matrix<Value, ROWS, COLS>;

        [[nodiscard]] auto operator()(this Kalman& self,
                                      Matrix<1UL, INPUTS> const& input,
                                      Matrix<1UL, MEASUREMENTS> const& measurement) -> Matrix<STATES, 1UL>
        {
            try {
                self.predict(input);
                self.correct(measurement);
                return self.state;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        auto predict(this Kalman& self, Matrix<1UL, INPUTS> const& input) -> Matrix<STATES, 1UL>
        {
            try {
                self.state = (self.state_transition * self.state) + (self.input_transition * input);
                self.state_covariance =
                    (self.state_transition * self.state_covariance * matrix_transpose(self.state_transition)) +
                    self.process_noise;
                return self.state;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        auto correct(this Kalman& self, Matrix<1UL, MEASUREMENTS> const& measurement) -> Matrix<STATES, 1UL>
        {
            try {
                auto const innovation{measurement - (self.measurement_transition * self.state)};
                auto const residual_covariance{(self.measurement_transition * self.state_covariance *
                                                matrix_transpose(self.measurement_transition)) +
                                               self.measurement_covariance};
                auto const kalman_gain{self.state_covariance * matrix_transpose(self.measurement_transition) *
                                       matrix_inverse(residual_covariance)};
                self.state = self.state + (kalman_gain * innovation);
                self.state_covariance =
                    (make_eye<Value, STATES>() - kalman_gain * self.measurement_transition) * self.state_covariance;
                return self.state;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        Matrix<STATES, 1UL> state{};

        Matrix<STATES, STATES> state_transition{};
        Matrix<STATES, STATES> state_covariance{};

        Matrix<STATES, INPUTS> input_transition{};
        Matrix<INPUTS, INPUTS> input_covariance{};

        Matrix<MEASUREMENTS, STATES> measurement_transition{};
        Matrix<MEASUREMENTS, MEASUREMENTS> measurement_covariance{};

        Matrix<STATES, STATES> process_noise{};
    };

}; // namespace Linalg

#endif // KALMAN_HPP