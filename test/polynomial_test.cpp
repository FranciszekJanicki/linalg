#include "polynomial.hpp"
#include <stdfloat>

auto main([[maybe_unused]] int const argc, [[maybe_unused]] char const* const argv[]) -> int
{
    using Polynomial = Linalg::Polynomial<std::float32_t, 2UL>;
    using Coeffs = Polynomial::Coeffs;

    for (Polynomial polynomial{Coeffs{1.0F, 1.0F, 2.0F}}; auto const root : polynomial.roots()) {
        fmt::println("Real: {}, Imag: {}", root.real(), root.imag());
    }

    return 0;
}