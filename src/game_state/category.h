#pragma once

#include <cstddef>

//Enum class for all yahtzee categories
enum class Category : size_t {
    Ones = 0,
    Twos = 1,
    Threes = 2,
    Fours = 3,
    Fives = 4,
    Sixes = 5,
    ThreeOfAKind = 6,
    FourOfAKind = 7,
    FullHouse = 8,
    SmallStraight = 9,
    LargeStraight = 10,
    Yahtzee = 11,
    Chance = 12
};

constexpr size_t NUM_CATEGORIES = 13;

// Function to convert Category enum to string
inline const char* CategoryToString(Category category) {
    switch (category) {
        case Category::Ones: return "Ones";
        case Category::Twos: return "Twos";
        case Category::Threes: return "Threes";
        case Category::Fours: return "Fours";
        case Category::Fives: return "Fives";
        case Category::Sixes: return "Sixes";
        case Category::ThreeOfAKind: return "Three of a Kind";
        case Category::FourOfAKind: return "Four of a Kind";
        case Category::FullHouse: return "Full House";
        case Category::SmallStraight: return "Small Straight";
        case Category::LargeStraight: return "Large Straight";
        case Category::Yahtzee: return "Yahtzee";
        case Category::Chance: return "Chance";
        default: return "Unknown Category";
    }
}