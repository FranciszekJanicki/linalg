#ifndef LQR_FINITE_HPP
#define LQR_FINITE_HPP

#include "stack_matrix.hpp"
#include <algorithm>
#include <concepts>
#include <ranges>
#include <vector>

namespace Linalg::Regulators {

    template <std::floating_point Value, std::size_t STATES, std::size_t INPUTS = 1UL, std::size_t MEASUREMENTS = 1UL>
    struct LQR_Finite {
        template <std::size_t ROWS, std::size_t COLS>
        using Matrix = Stack::Matrix<Value, ROWS, COLS>;
        using RicattiSolutions = std::vector<Matrix>;
        using namespace Stack;

        static auto get_ricatti_solutions(Matrix<STATES, STATES> const& state_transition,
                                          Matrix<STATES, INPUTS> const& input_transition,
                                          Matrix<STATES, STATES> const& state_cost,
                                          Value const input_cost,
                                          Matrix<STATES, STATES> const& end_condition,
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

        static auto get_ricatti_solution(Matrix<STATES, STATES> const& state_transition,
                                         Matrix<STATES, INPUTS> const& input_transition,
                                         Matrix<STATES, STATES> const& state_cost,
                                         Value const input_cost,
                                         Matrix<STATES, STATES> const& prev_solution) -> Matrix
        {
            return -1 * (prev_solution * state_transition -
                         prev_solution * input_transition * matrix_inverse(input_cost) *
                             matrix_transpose(input_transition) * prev_solution +
                         matrix_transpose(state_transition) * prev_solution + state_cost);
        }

        static auto get_optimal_gain(std::uint64_t const sample,
                                     Matrix<STATES, STATES> const& ricatti,
                                     Matrix<STATES, INPUTS> const& input_transition,
                                     Value const input_cost) -> Matrix<1UL, STATES>
        {
            return matrix_inverse(input_cost) * matrix_transpose(input_transition) * ricatti;
        }

        [[nodiscard]] auto operator()(this LQR_Finite& self,
                                      Matrix<1UL, INPUTS> const& input,
                                      Matrix<1UL, MEASUREMENTS> const& measurement) -> Matrix<STATES, 1UL>
        {
            auto error{input - measurement};
            return input - (get_optimal_gain(sample,
                                             self.ricatti_solutions[self.sample++],
                                             self.input_transition,
                                             self.input_cost) *
                            error);
        }

        Matrix<STATES, STATES> state_transition{};
        Matrix<STATES, INPUTS> input_transition{};
        Matrix<STATES, STATES> state_cost{};
        Value input_cost_{};
        RicattiSolutions ricatti_solutions{};
        std::uint64_t sample{0UL};
    };

}; // namespace Linalg::Regulators

#endif // LQR_FINITE_HPP