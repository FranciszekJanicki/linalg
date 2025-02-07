#include "filters.hpp"
#include <array>
#include <stdfloat>
#include <print>

int main([[maybe_unused]] int const argc, [[maybe_unused]] char const * const argv[])
{

    using namespace Linalg::Filters;

    auto fir{make_fir_filter(std::array{0.2F32, 0.2F32, 0.2F32})};

    for (auto measurement : {0.1F32, 0.2F32, 0.3F32, 0.4F32, 0.5F32}) {
        std::print("Estimate: {}", fir(measurement));
    }

    return 0;
}
