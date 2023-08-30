/**
 * @file stat.hpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Tool class for time test
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#pragma once
#include <chrono>
#include <mutex>
#include <vector>
#include "stat.hpp"

/**
 * @brief Time test class
 */
class TimeTest {
    using clock_source = std::chrono::high_resolution_clock;

    // variables to record time elapse of each run
    typename clock_source::time_point this_time_start_, this_time_stop_;

    // Statistics of all runs
    Stat<clock_source::duration> stats_;

 public:
    /**
     * @brief Get statistics of each run
     *
     * @return const Stat<clock_source::duration>& the statistics
     */
    const Stat<clock_source::duration>& stats() const { return stats_; }

    /**
     * @brief Reset state, clear all statistics
     */
    void reset() { stats_.reset(); }

    /**
     * @brief record time on the start of a run
     *
     * @note Only the latest start call functions before a stop call
     */
    __attribute__((always_inline)) inline void start() {
        this_time_start_ = clock_source::now();
    }

    /**
     * @brief record time on the end of a run and update statistics
     */
    __attribute__((always_inline)) inline void stop() {
        this_time_stop_ = clock_source::now();
        stats_.add(this_time_stop_ - this_time_start_);
    }

    TimeTest() { reset(); }
};
