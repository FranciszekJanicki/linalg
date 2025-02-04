#ifndef ADRC_HPP
#define ADRC_HPP

#include <concepts>

namespace Linalg::Regulators {

    template <std::floating_point Value>
    struct ADRC {};

}; // namespace Linalg::Regulators

#endif // ADRC_HPP