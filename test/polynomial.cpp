#include "polynomial.hpp"
#include <stdfloat>

int main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[])
{
    using namespace Linalg;

    Polynomial<std::float32_t, 2UL> polynomial{2.0F32, 4.0F32, 4.0F32};
    print<std::float32_t, 2UL>(polynomial);
    print_roots<std::float32_t, 2UL>(polynomial);

    return 0;
}