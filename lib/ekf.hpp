#ifndef EKF_HPP
#define EKF_HPP

#include "common.hpp"
#include "stack_matrix.hpp"

namespace Linalg::Observers {

    template <Arithmetic Value, Size STATES, Size CONTROLS = 1UL, Size MEASUREMENTS = 1UL>
    struct EKF {
        template <Size ROWS, Size COLS>
        using Matrix = Stack::Matrix<Value, ROWS, COLS>;

        Matrix<STATES, 1UL> state{};
        Matrix<STATES, STATES> state_covariance{};

        Matrix<STATES, STATES> state_transition{};
        Matrix<STATES, CONTROLS> control_transition{};
        Matrix<STATES, MEASUREMENTS> measurement_transition{};

        Matrix<MEASUREMENTS, MEASUREMENTS> measurement_noise{};
        Matrix<STATES, STATES> process_noise{};
    };

}; // namespace Linalg::Observers

#endif // EKF_HPP