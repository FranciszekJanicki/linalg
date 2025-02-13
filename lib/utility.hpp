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

    template <std::floating_point T>
    [[nodiscard]] T degrees_to_radians(T const degrees) noexcept
    {
        return degrees * static_cast<T>(M_PI) / static_cast<T>(360.0);
    }

    template <std::floating_point T>
    [[nodiscard]] T radians_to_degrees(T const radians) noexcept
    {
        return radians * static_cast<T>(360.0) / static_cast<T>(M_PI);
    }

    template <std::floating_point Float, std::integral Int>
    [[nodiscard]] Int degrees_to_steps(Float const degrees, Int const steps_per_360) noexcept
    {
        return static_cast<Int>(degrees) * steps_per_360 / static_cast<Int>(360.0);
    }

    template <std::floating_point Float, std::integral Int>
    [[nodiscard]] Int degree_diff_to_step_diff(Float const degree_diff, Int const steps_per_360) noexcept
    {
        return (static_cast<Int>(degrees_to_steps(degree_diff, steps_per_360)) + steps_per_360) % steps_per_360;
    }

    template <std::integral Int, std::floating_point Float>
    [[nodiscard]] Float steps_to_degrees(Int const steps, Int const steps_per_360) noexcept
    {
        return static_cast<Float>(steps) * static_cast<Float>(360.0) / steps_per_360;
    }

    template <std::floating_point Float, std::integral Int>
    [[nodiscard]] Float step_diff_to_degree_diff(Int const step_diff, Int const steps_per_360) noexcept
    {
        return std::modulus<Float>{}(steps_to_degrees(step_diff, steps_per_360) + static_cast<Float>(360.0),
                                     static_cast<Float>(360.0));
    }

    template <typename Value>
    [[nodiscard]] std::array<std::uint8_t, sizeof(Value)> value_to_bytes(Value const& value) noexcept
    {
        std::array<std::uint8_t, sizeof(Value)> bytes{};
        std::memcpy(bytes.data(), std::addressof(value), sizeof(Value));
        return bytes;
    }

    template <typename Value>
    [[nodiscard]] Value bytes_to_value(std::array<std::uint8_t, sizeof(Value)> const& bytes) noexcept
    {
        Value value{};
        std::memcpy(std::addressof(value), bytes.data(), sizeof(Value));
        return value;
    }

}; // namespace Linalg::Utility

#endif // UTILITY_HPP