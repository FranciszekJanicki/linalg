#include "transforms.hpp"
#include <array>
#include <print>
#include <stdfloat>

auto main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[]) -> int
{
    using namespace Linalg;

    auto result = Transform::idft(Transform::dft(std::array{0.0, 1.0, 2.0, 3.0}));

    std::print("{}", result[0]);

    return 0;
}