#include "dice.h"
#include <stdexcept>
#include <numeric>

Dice::Dice() : counts_{0, 0, 0, 0, 0, 0} {}

Dice::Dice(const std::vector<size_t>& values) : Dice() {
    for (size_t value : values) {
        add_die(value);
    }
}

Dice::Dice(std::initializer_list<size_t> values) : Dice() {
    for (size_t value : values) {
        add_die(value);
    }
}

size_t Dice::operator[](size_t value) const {
    if (value < 1 || value > 6) {
        throw std::out_of_range("Dice value must be between 1 and 6");
    }
    return counts_[value - 1];
}

size_t Dice::total() const {
    return std::accumulate(counts_.begin(), counts_.end(), 0);
}

size_t Dice::sum() const {
    size_t total_sum = 0;
    for (size_t i = 0; i < 6; ++i) {
        total_sum += (i + 1) * counts_[i];
    }
    return total_sum;
}

void Dice::reset() {
    counts_.fill(0);
}

void Dice::add_die(size_t value) {
    if (value < 1 || value > 6) {
        throw std::invalid_argument("Dice value must be between 1 and 6");
    }
    counts_[value - 1]++;
}

const std::array<size_t, 6>& Dice::counts() const {
    return counts_;
}