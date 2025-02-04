#ifndef FILTERS_HPP
#define FILTERS_HPP

#include <algorithm>
#include <concepts>
#include <functional>
#include <queue>
#include <ranges>
#include <utility>

namespace Linalg::Filters {

    template <std::floating_point Value>
    using Filter = std::function<Value(Value)>;

    template <std::floating_point Value, std::size_t SAMPLES>
    [[nodiscard]] constexpr auto make_fir_filter(std::array<Value, SAMPLES> const& coeffs) noexcept
    {
        return [coeffs, measurements = std::array<Value, SAMPLES>](Value const measurement) mutable {
            Value estimate{0.0};
            for (auto &[coeff, measurement] : std::views::zip(coeffs, measurements)) {
                estimate += coeff * measurement;
            }            
            std::shift_right(measurements.begin(), measurements.end());
            measurements[0UL] = measurement;
            return estimate;
        };
    }

    template <std::floating_point Value, std::size_t SAMPLES>
    [[nodiscard]] constexpr auto make_iir_filter(std::array<Value, SAMPLES> const& num_coeffs,
                                                 std::array<Value, SAMPLES> const& den_coeffs) noexcept
    {
        return [measurements = std::array<Value, SAMPLES>{},
                estimates = std::array<Value, SAMPLES>{},
                num_coeffs,
                den_coeffs](Value const measurement) mutable {
            Value estimate{0.0};
            for (auto &[num_coeff, den_coeff, measurement, estimate] :
                 std::views::zip(num_coeffs, den_coeffs, measurements, estimates)) {
                estimate += num_coeff * measurement - den_coeff * estimate;
            }
            std::shift_right(estimates.begin(), estimates.end());
            std::shift_right(measurements.begin(), measurements.end());
            estimates[0UL] = estimate;
            measurements[0UL] = measurement;
            return estimate;
        };
    }

}; // namespace Linalg::Filters

#endif // FILTERS_HPP