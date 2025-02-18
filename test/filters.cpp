#include "kalman.hpp"
#include <array>
#include <print>
#include <stdfloat>

int main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[])
{
    using namespace Linalg::Filters;
    using Kalman = Kalman<std::float32_t, 2UL>;
    using Matrix2x2 = Kalman::Matrix<2UL, 2UL>;
    using Matrix1x2 = Kalman::Matrix<1UL, 2UL>;
    using Matrix2x1 = Kalman::Matrix<2UL, 1UL>;
    using Matrix1x1 = Kalman::Matrix<1UL, 1UL>;

    auto const dt{1.0F32};
    auto const sigma_perc{1.0F32};
    auto const sigma_move{0.02F32};

    Kalman kalman{.state = Matrix2x1{{0.0F32}, {0.0F32}},
                  .state_covariance = Matrix2x2{{1.0F32, 0.0F32}, {0.0F32, 10.0F32}},
                  .state_transition = Matrix2x2{{1.0F32, dt}, {0.0F32, 1.0F32}},
                  .control_transition = Matrix2x1{{1.0F32}, {1.0F32}},
                  .measurement_transition = Matrix1x2{{1.0F32, 0.0F32}},
                  .measurement_noise = Matrix1x1{{std::pow(sigma_perc, 2.0F32)}},
                  .process_noise = Matrix2x2{{0.25F32 * std::pow(dt, 4.0F32), 0.5F32 * std::pow(dt, 3.0F32)},
                                             {0.5F32 * std::pow(dt, 3.0F32), std::pow(dt, 2.0F32)}} *
                                   std::pow(sigma_move, 2.0F32)};

    for (auto i{0UL}; i < 100UL; ++i)
        try {
            auto const kalman_state = kalman(Matrix1x1{{1.0F}}, Matrix1x1{{1.0F}});
            kalman_state.print();
        } catch (std::runtime_error const& error) {
            std::print("error: {}", error.what());
        }

    return 0;
}
