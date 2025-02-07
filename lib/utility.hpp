#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <concepts>
#include <numeric>
#include <stdexcept>

namespace Linalg::Utility {

    template <std::floating_point T>
    [[nodiscard]] T differentiate(T const value,
                                  T const prev_value,
                                  T const sampling_time,
                                  T const prev_derivative,
                                  T const time_constant)
    {
        if (time_constant + sampling_time == static_cast<T>(0)) {
            throw std::runtime_error{"Division by 0!"};
        }
        return (value - prev_value + prev_derivative * time_constant) / (time_constant + sampling_time);
    }

    template <std::floating_point T>
    [[nodiscard]] T differentiate(T const value, T const prev_value, T const sampling_time)
    {
        if (sampling_time == static_cast<T>(0)) {
            throw std::runtime_error{"Division by 0!"};
        }
        return (value - prev_value) / sampling_time;
    }

    template <std::floating_point T>
    [[nodiscard]] T integrate(T const value, T const prev_value, T const sampling_time) noexcept
    {
        return (value + prev_value) * static_cast<T>(0.5F) * sampling_time;
    }

}; // namespace Linalg::Utility

#endif // UTILITY_HPP