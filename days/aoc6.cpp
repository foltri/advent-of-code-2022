//
// Created by Balint Zsiga on 2022. 12. 07..
//

#include "aoc6.h"

namespace aoc6
{
    // all chars are redundantly put in a set a better solution would only add the new char which would result in less comparisons
    bool allDifferent(std::string_view str)
    {
        std::set<char> charSet{};
        for (auto c : str) charSet.insert(c);
        return charSet.size() == str.length();
    }

    void run()
    {
        constexpr int markerLength{14};

        std::string str { helper::readInput("../input/aoc6.txt")[0] };

        size_t index{0};
        for (size_t i{markerLength}; i<str.length(); ++i)
        {
            if (allDifferent(str.substr(i - markerLength, markerLength))) { index = i; break; };
        }

        std::cout << "The market was detected at index: " << index << '\n';
    }
}
