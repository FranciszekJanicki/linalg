#ifndef LQR_INFINITE_HPP
#define LQR_INFINITE_HPP

#include "stack_matrix.hpp"
#include <concepts>

namespace Linalg::Regulators {

    template <std::floating_point T, std::size_t STATES, std::size_t INPUTS = 1UL>
    struct LQR_Infinite {
    public:
        template <std::size_t ROWS, std::size_t COLS>
        using Matrix = Stack::Matrix<T, ROWS, COLS>;

        [[nodiscard]] Matrix<STATES, 1UL>
        operator()(this LQR_Infinite& self, Matrix<1UL, INPUTS> const& input, Matrix<STATES, 1UL> const& state)
        {
            try {
                return input - self.state_gain * state;
            } catch (std::runtime_error const& error) {
                throw error;
            }
        }

        Matrix<1UL, STATES> state_gain{};
    };

}; // namespace Linalg::Regulators

#endif // LQR_INFINITE_HPP