/**
 * @file stat.hpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Tool class for min/max/avg statistics
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#pragma once
#include <functional>
#include <limits>

/**
 * @brief min/max/avg statistics class
 *
 * @tparam ElementType element type, supposing it supports operation += and
 * default constructor
 */
template <typename ElementType>
class Stat {
 public:
    /**
     * @brief Insert a new data x, updating min/max/avg
     *
     * @note average value is calculated by recording count and total value
     *
     * @warning overflow of total value is not cared
     *
     * @param x new data
     */
    void add(ElementType x) {
        total_ += x;
        if (count_ == 0 || min_ > x)
            min_ = x;
        if (count_ == 0 || max_ < x)
            max_ = x;
        ++count_;
    }

    /**
     * @brief Reset state, clear all records
     */
    void reset() {
        total_ = ElementType();
        // min_ = std::numeric_limits<ElementType>::max();
        // max_ = std::numeric_limits<ElementType>::min();
        count_ = 0;
    }

    /**
     * @brief Get min value
     *
     * @return ElementType min value
     */
    ElementType min() const { return min_; }

    /**
     * @brief Get max value
     *
     * @return ElementType max value
     */
    ElementType max() const { return max_; }

    /**
     * @brief Get average value
     *
     * @return ElementType average value
     */
    ElementType avg() const { return total_ / count_; }

    /**
     * @brief Convert recorded values to human-readable string
     *
     * @param fmt formatter for element, users can suffix units to improve
     * readability
     * @return std::string human-readable string of records
     */
    std::string to_string(std::function<std::string(ElementType)> fmt) const {
        return std::string("max=") + fmt(max()) + ", min=" + fmt(min()) +
               ", avg=" + fmt(avg());
    }

    Stat() { reset(); }

 private:
    // sum of all elements, used with count_ to calculate average
    ElementType total_;

    // minimum of elements
    ElementType min_;

    // maximum of elements
    ElementType max_;

    // count of elements, used with total_ to calculate average
    size_t count_;
};
