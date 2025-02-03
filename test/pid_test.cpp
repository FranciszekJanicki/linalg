#include "pid.hpp"
#include <array>
#include <ranges>

auto main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[]) -> int
{
    using namespace Linalg::Regulators;

    std::array<float, 100UL> inputs{};
    for (auto [index, input] : std::views::enumerate(inputs)) {
        input = index;
    }

    PID pid{.kp = 100.0F, .ki = 0.0F, .kd = 0.0F, .kc = 0.0F, .saturation = 100.0F};

    auto output = 0.0F;
    auto dt = 0.01F;

    for (auto input : inputs) {
        auto control{pid(input - output, dt)};
        fmt::println("{}", control);
        output += control > output ? 1.0F : -1.0F;

    }
}