#include "quaternion3d.hpp"
#include "stack_kalman.hpp"
#include "stack_matrix.hpp"
#include "stack_vector.hpp"
#include "vector3d.hpp"
#include <fmt/core.h>
#include <functional>
#include <stdfloat>

auto main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[]) -> int
{
    using namespace Linalg::Stack;
    using Kalman = Kalman<std::float32_t, 2UL>;
    using Matrix2x2 = Matrix<std::float32_t, 2UL, 2UL>;
    using Matrix1x2 = Matrix<std::float32_t, 1UL, 2UL>;
    using Matrix2x1 = Matrix<std::float32_t, 2UL, 1UL>;
    using Matrix1x1 = Matrix<std::float32_t, 1UL, 1UL>;

    auto const dt{1.0F32};
    auto const sigma_perc{1.0F32};
    auto const sigma_move{1.0F32};

    Kalman kalman{.state = Matrix2x1{{1.0F32}, {0.0F32}},
                  .state_transition = Matrix2x2{{1.0F32, dt}, {0.0F32, 1.0F32}},
                  .state_covariance = Matrix2x2{{1.0F32, 0.0F32}, {0.0F32, 1.0F32}},
                  .input_transition = Matrix2x1{{1.0F32}, {1.0F32}},
                  .input_covariance = Matrix1x1{{1.0F32}},
                  .measurement_transition = Matrix1x2{{1.0F32, 0.0F32}},
                  .measurement_covariance = Matrix1x1{{std::pow(sigma_perc, 2.0F32)}},
                  .process_noise = Matrix2x2{{0.25F32 * std::pow(dt, 4.0F32), 0.5F32 * std::pow(dt, 3.0F32)},
                                             {0.5F32 * std::pow(dt, 3.0F32), std::pow(dt, 2.0F32)}} *
                                   std::pow(sigma_move, 2.0F32)};

    for (auto i{0UL}; i < 100UL; ++i)
        try {
            auto const kalman_state = std::invoke(kalman, Matrix1x1{{0.0F32}}, Matrix1x1{{0.0F32}});
            kalman_state.print();
        } catch (std::runtime_error const& error) {
            fmt::print("error: {}", error.what());
        }

    return 0;
}
