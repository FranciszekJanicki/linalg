#ifndef REGULATOR_HPP
#define REGULATOR_HPP

#include "adrc.hpp"
#include "binary.hpp"
#include "lqr.hpp"
#include "pid.hpp"
#include "ternary.hpp"
#include <functional>
#include <memory>
#include <utility>
#include <variant>

namespace Regulators {

    enum struct Algorithm {
        PID,
        LQR,
        ADRC,
        BINARY,
        TERNARY,
    };

}; // namespace Regulators

#endif // REGULATOR_HPP