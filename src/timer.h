#ifndef TIMER_H
#define TIMER_H
#include <chrono>
#include <cstdint>

class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> lifetime;

public:
    // Define types
    using Nanos = std::chrono::nanoseconds;
    using Micros = std::chrono::microseconds;
    using Millis = std::chrono::milliseconds;
    using Seconds = std::chrono::seconds;
    using Minutes = std::chrono::minutes;
    using Hours = std::chrono::hours;

    // -----
    // CONSTRUCTORS
    // -----
    Timer() {   // Start timer on construction
        lifetime = std::chrono::steady_clock::now();
    }

    // -----
    // FUNCTIONS
    // -----
    void restart() {    // Restart timer
        lifetime = std::chrono::steady_clock::now();
    }

    template <typename T>
    uint64_t click() {  // Get elapsed time and restart timer
        uint64_t time = std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - lifetime).count();
        restart();
        return time;
    }

    template <typename T>
    const uint64_t glance() {   // Get elapsed time without restarting
        return std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - lifetime).count();
    }

};
#endif  // TIMER_H