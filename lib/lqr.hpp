#ifndef LQR_HPP
#define LQR_HPP

#include "common.hpp"
#include "matrix.hpp"
#include <algorithm>
#include <ranges>
#include <vector>

namespace Regulators {

    template <Linalg::Arithmetic Value>
    struct LQR

    {
    public:
        using Matrix = Linalg::Matrix<Value>;
        using RicattiSolutions = std::vector<Matrix>;

        constexpr LQR(Matrix const& state_transition,
                      Matrix const& input_transition,
                      Matrix const& state_cost,
                      Matrix const& input_cost,
                      Matrix const& end_condition,
                      std::uint64_t const samples) :
            state_transition_{state_transition},
            input_transition_{input_transition},
            state_cost_{state_cost},
            input_cost_{input_cost},
            ricatti_solutions_{get_ricatti_solutions(state_transition,
                                                     input_transition,
                                                     state_cost,
                                                     input_cost,
                                                     end_condition,
                                                     samples)}
        {}

        constexpr LQR(Matrix&& state_transition,
                      Matrix&& input_transition,
                      Matrix&& state_cost,
                      Matrix&& input_cost,
                      Matrix const& end_condition,
                      std::uint64_t const samples) noexcept :
            state_transition_{std::forward<Matrix>(state_transition)},
            input_transition_{std::forward<Matrix>(input_transition)},
            state_cost_{std::forward<Matrix>(state_cost)},
            input_cost_{std::forward<Matrix>(input_cost)},
            ricatti_solutions_{get_ricatti_solutions(state_transition,
                                                     input_transition,
                                                     state_cost,
                                                     input_cost,
                                                     end_condition,
                                                     samples)}
        {}

        [[nodiscard]]
        inline auto
        operator()(this LQR& self, std::uint64_t const sample, Matrix const& input, Matrix const& measurement) -> Matrix
        {
            auto error{input - measurement};
            return input - (get_optimal_gain(sample,
                                             self.ricatti_solutions_[sample],
                                             self.input_transition_,
                                             self.input_cost_) *
                            error);
        }

    private:
        static inline auto get_optimal_gain(std::uint64_t const sample,
                                            Matrix const& ricatti,
                                            Matrix const& input_transition,
                                            Matrix const& input_cost) -> Matrix
        {
            return Matrix::inverse(input_cost).value() * Matrix::transpose(input_transition) * ricatti;
        }

        static inline auto get_ricatti_solutions(Matrix const& state_transition,
                                                 Matrix const& input_transition,
                                                 Matrix const& state_cost,
                                                 Matrix const& input_cost,
                                                 Matrix const& end_condition,
                                                 std::uint64_t const samples) -> RicattiSolutions
        {
            RicattiSolutions solutions{};
            solutions.reserve(samples);
            solutions.push_back(end_condition);
            for (std::uint64_t i{}; i < samples; ++i) {
                solutions.push_back(
                    get_ricatti_solution(state_transition, input_transition, state_cost, input_cost, solutions.back()));
            }
            std::ranges::reverse(solutions);
            return solutions;
        }

        static inline auto get_ricatti_solution(Matrix const& state_transition,
                                                Matrix const& input_transition,
                                                Matrix const& state_cost,
                                                Matrix const& input_cost,
                                                Matrix const& prev_solution) -> Matrix
        {
            return -1 * (prev_solution * state_transition -
                         prev_solution * input_transition * Matrix::inverse(input_cost).value() *
                             Matrix::transpose(input_transition) * prev_solution +
                         Matrix::transpose(state_transition) * prev_solution + state_cost);
        }

        Matrix state_transition_{};
        Matrix input_transition_{};
        Matrix state_cost_{};
        Matrix input_cost_{};

        RicattiSolutions ricatti_solutions_{};
    };

}; // namespace Regulators

#endif // LQR_HPP