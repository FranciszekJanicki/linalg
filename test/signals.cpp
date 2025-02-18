#include "signals.hpp"
#include <print>

int main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[])
{
    using namespace Linalg::Signals;

    auto const points = std::vector{Point{0U, 0.0F}, Point{1U, 10.0F}, Point{2U, 20.0F}, Point{3U, 30.0F}};
    auto const sampling_time_ms = 1U;
    auto const iterations = (points.back().time_ms - points.front().time_ms) / sampling_time_ms;

    auto interpolator = make_interpolator_1D(points, sampling_time_ms);

    for (auto i = 0U; i < iterations; ++i) {
        try {
            std::println("Interpolated: {}", interpolator());
        } catch (std::runtime_error const& error) {
            std::println("error: {}", error.what());
        }
    }

    return 0;
}
