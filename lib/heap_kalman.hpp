#ifndef HEAP_KALMAN_HPP
#define HEAP_KALMAN_HPP

#include "common.hpp"
#include "heap_matrix.hpp"
#include <fmt/core.h>
#include <stdexcept>
#include <utility>

namespace Linalg::Heap {

    template <Arithmetic Value>
    struct Kalman {
    public:
        using Matrix = Matrix<Value>;

        [[nodiscard]] auto operator()(this Kalman& self, Matrix const& input, Matrix const& measurement) -> Matrix
        {
            try {
                self.predict(input);
                self.correct(measurement);
                return self.state_;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

    private:
        auto predict(this Kalman& self, Matrix const& input) -> Matrix
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

        auto correct(this Kalman& self, Matrix const& measurement) -> Matrix
        {
            try {
                auto const innovation{measurement - (self.measurement_transition * self.state)};
                auto const residual_covariance{(self.measurement_transition * self.state_covariance *
                                                matrix_transpose(self.measurement_transition)) +
                                               self.measurement_covariance_};
                auto const kalman_gain{self.state_covariance * matrix_transpose(self.measurement_transition) *
                                       matrix_inverse(residual_covariance)};
                self.state = self.state + (kalman_gain * innovation);
                self.state_covariance =
                    (make_eye<Value, STATES>() - kalman_gain * self.measurement_transition) * self.state_covariance;
                return self.state_;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        Matrix state{};

        Matrix state_transition{};
        Matrix state_covariance{};

        Matrix input_transition{};
        Matrix input_covariance{};

        Matrix measurement_transition{};
        Matrix measurement_covariance{};

        Matrix process_noise{};
    };

}; // namespace Linalg::Heap

#endif // HEAP_KALMAN_HPP