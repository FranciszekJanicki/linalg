#ifndef LQR_INFINITE_HPP
#define LQR_INFINITE_HPP

#include "common.hpp"
#include "stack_matrix.hpp"

namespace Linalg {

    template <Arithmetic Value, Size STATES, Size INPUTS = 1UL>
    struct LQR_Infinite {
    public:
        template <Size ROWS, Size COLS>
        using Matrix = Stack::Matrix<Value, ROWS, COLS>;

        [[nodiscard]] inline auto operator()(this LQR_Infinite& self,
                                             Matrix<1UL, INPUTS> const& input,
                                             Matrix<STATES, 1UL> const& state) -> Matrix<STATES, 1UL>
        {
            try {
                return input - self.state_gain * state;
            } catch (Error const& error) {
                throw error;
            }
        }

        Matrix<1UL, STATES> state_gain{};
    };

}; // namespace Linalg

#endif // LQR_INFINITE_HPP