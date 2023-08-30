#pragma once
#include <functional>
#include <limits>

template <typename ElementType>
class Stat {
 public:
    void add(ElementType x) {
        total_ += x;
        if (count_ == 0 || min_ > x)
            min_ = x;
        if (count_ == 0 || max_ < x)
            max_ = x;
        ++count_;
    }
    void reset() {
        total_ = ElementType();
        // min_ = std::numeric_limits<ElementType>::max();
        // max_ = std::numeric_limits<ElementType>::min();
        count_ = 0;
    }
    ElementType min() const { return min_; }
    ElementType max() const { return max_; }
    ElementType avg() const { return total_ / count_; }
    std::string to_string(std::function<std::string(ElementType)> fmt) const {
        return std::string("max=") + fmt(max()) + ", min=" + fmt(min()) +
               ", avg=" + fmt(avg());
    }

    Stat() { reset(); }

 private:
    ElementType total_, min_, max_;
    size_t count_;
};
