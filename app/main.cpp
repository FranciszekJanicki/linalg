#include "kalman.hpp"
#include "quaternion3d.hpp"
#include "stack_matrix.hpp"
#include "stack_vector.hpp"
#include "vector3d.hpp"
#include <fmt/core.h>
#include <functional>
#include <stdfloat>

auto main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[]) -> int
{
    using namespace Linalg;
    using Kalman = Kalman<std::float_t, 2UL>;
    using Matrix2x2 = Kalman::Matrix<2UL, 2UL>;
    using Matrix1x2 = Kalman::Matrix<1UL, 2UL>;
    using Matrix2x1 = Kalman::Matrix<2UL, 1UL>;
    using Matrix1x1 = Kalman::Matrix<1UL, 1UL>;

    auto const dt{1.0F};
    auto const sigma_perc{1.0F};
    auto const sigma_move{0.02F};

    Kalman kalman{.state = Matrix2x1{{1.0F}, {10.0F}},
                  .state_transition = Matrix2x2{{1.0F, dt}, {0.0F, 1.0F}},
                  .state_covariance = Matrix2x2{{1.0F, 0.0F}, {0.0F, 10.0F}},
                  .input_transition = Matrix2x1{{1.0F}, {1.0F}},
                  .input_covariance = Matrix1x1{{1.0F}},
                  .measurement_transition = Matrix1x2{{1.0F, 0.0F}},
                  .measurement_covariance = Matrix1x1{{std::pow(sigma_perc, 2.0F)}},
                  .process_noise = Matrix2x2{{0.25F * std::pow(dt, 4.0F), 0.5F * std::pow(dt, 3.0F)},
                                             {0.5F * std::pow(dt, 3.0F), std::pow(dt, 2.0F)}} *
                                   std::pow(sigma_move, 2.0F)};

    for (auto i{0UL}; i < 100UL; ++i)
        try {
            auto const kalman_state = std::invoke(kalman, Matrix1x1{{1.0F}}, Matrix1x1{{1.0F}});
            kalman_state.print();
        } catch (std::runtime_error const& error) {
            fmt::print("error: {}", error.what());
        }

    return 0;
}
