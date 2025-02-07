#include "filters.hpp"
#include <array>
#include <print>
#include <stdfloat>

int main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[])
{
    using namespace Linalg::Filters;

    auto fir{make_fir_filter(std::array{0.2F32, 0.2F32, 0.2F32, 0.2F32, 0.2F32})};

    for (auto i{0UL}; i < 10000UL; ++i) {
        // std::print("Estimate: {}", fir(i));
    }

    return 0;
}
