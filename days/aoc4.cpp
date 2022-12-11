//
// Created by Balint Zsiga on 2022. 12. 05..
//

#include "aoc4.h"

namespace aoc4
{
    struct SectionAssignment
    {
        int lower{};
        int higher{};
    };

    bool isContained(const SectionAssignment& a1, const SectionAssignment& a2)
    {
        // a1 contains a2
        if (a1.lower <= a2.lower && a1.higher >= a2.higher) return true;
        // a2 contains a1
        if (a2.lower <= a1.lower && a2.higher >= a1.higher) return true;

        return false;
    }

    bool isIntersecting(const SectionAssignment& a1, const SectionAssignment& a2)
    {
        // a1 lower is in the range of a2
        if (a1.lower >= a2.lower && a1.lower <= a2.higher) return true;
        // a1 higher is in the range of a2
        if (a1.higher >= a2.lower && a1.higher <= a2.higher) return true;
        // a2 lower is in the range of a1
        if (a2.lower >= a1.lower && a2.lower <= a1.higher) return true;
        // a2 higher is in the range of a1
        if (a2.higher >= a1.lower && a2.higher <= a1.higher) return true;

        return false;
    }

    SectionAssignment parseSection(std::string_view str)
    {
        std::string strLower { str.substr(0, str.find('-')) };
        int lower { std::stoi(strLower) };

        str.remove_prefix(strLower.length() + 1);
        std::string strHigher { str };
        int higher { std::stoi(strHigher) };

        return { lower, higher };

    }

    std::pair<SectionAssignment, SectionAssignment> parseLine(std::string_view str)
    {
        size_t commaPos { str.find(',') };
        SectionAssignment a1 { parseSection(str.substr(0, commaPos)) };
        SectionAssignment a2 { parseSection(str.substr(commaPos + 1, str.length())) };

        return { a1, a2 };
    }

    void run()
    {
        int numContained{};
        int numIntersecting{};
        for (std::string_view line : helper::readInput("../input/aoc4.txt"))
        {
            std::pair<SectionAssignment, SectionAssignment> p { parseLine(line) };

            if (isContained(p.first, p.second)) numContained++;
            if (isIntersecting(p.first, p.second)) numIntersecting++;
        }

        std::cout << "The number of contained sections is "     << numContained     << '\n';
        std::cout << "The number of intersecting sections is "  << numIntersecting  << '\n';
    }
}
