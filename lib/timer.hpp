#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <cstdint>
#include <fmt/core.h>

struct Timer {
public:
    constexpr Timer() : time_{get_time()}
    {}

    constexpr ~Timer()
    {
        fmt::print("Elapsed time: {}", get_time() - time_);
    }

private:
    static inline auto get_time() -> std::time_t
    {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

    std::time_t time_{0UL};
};

#endif // TIMER_HPP
