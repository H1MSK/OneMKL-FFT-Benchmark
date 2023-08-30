#pragma once
#include <chrono>
#include <mutex>
#include <vector>
#include "stat.hpp"

class TimeTest {
    using clock_source = std::chrono::high_resolution_clock;
    typename clock_source::time_point this_time_start_, this_time_stop_;
    Stat<clock_source::duration> stats_;

 public:
    const Stat<clock_source::duration>& stats() const { return stats_; }

    void reset() { stats_.reset(); }

    __attribute__((always_inline)) inline void start() {
        this_time_start_ = clock_source::now();
    }

    __attribute__((always_inline)) inline void stop() {
        this_time_stop_ = clock_source::now();
        stats_.add(this_time_stop_ - this_time_start_);
    }

    TimeTest() { reset(); }
};
