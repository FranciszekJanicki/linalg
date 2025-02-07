#ifndef FILTERS_HPP
#define FILTERS_HPP

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <functional>
#include <queue>
#include <ranges>
#include <utility>

namespace Linalg::Filters {

    template <std::floating_point T>
    using Filter = std::function<T(T)>;

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] auto make_fir_filter(std::array<T, N> const& coeffs) noexcept
    {
        return [coeffs, measurements = std::array<T, N>{}](T const measurement) mutable {
            auto estimate = static_cast<T>(0.0);
            for (auto [coeff, measured] : std::views::zip(coeffs, measurements)) {
                estimate += coeff * measured;
            }
            std::shift_right(measurements.begin(), measurements.end(), 1UL);
            measurements[0UL] = measurement;
            return estimate;
        };
    }

    template <std::floating_point T, std::size_t N>
    [[nodiscard]] auto make_iir_filter(std::array<T, N> const& num_coeffs, std::array<T, N> const& den_coeffs) noexcept
    {
        return [measurements = std::array<T, N>{}, estimates = std::array<T, N>{}, num_coeffs, den_coeffs](
                   T const measurement) mutable {
            auto estimate = static_cast<T>(0.0);
            for (auto [num_coeff, den_coeff, measured, estiamted] :
                 std::views::zip(num_coeffs, den_coeffs, measurements, estimates)) {
                estimate += num_coeff * measured - den_coeff * estimate;
            }
            std::shift_right(estimates.begin(), estimates.end(), 1UL);
            std::shift_right(measurements.begin(), measurements.end(), 1UL);
            estimates[0UL] = estimate;
            measurements[0UL] = measurement;
            return estimate;
        };
    }

}; // namespace Linalg::Filters

#endif // FILTERS_HPP