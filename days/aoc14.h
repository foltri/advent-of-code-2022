//
// Created by Balint Zsiga on 2022. 12. 24..
//

#ifndef ADVENT_OF_CODE_2022_AOC14_H
#define ADVENT_OF_CODE_2022_AOC14_H

#include <vector>
#include <numeric>
#include <sstream>

#include "../helper.h"

namespace aoc14
{
    void run();

    struct RockBoundary
    {
        std::pair<int, int> position{};
        int index{};
    };

    class Cave
    {
    private:
        std::vector<std::vector<char>> m_data{};
        std::pair<int, int> m_hole{};
    public:
        explicit Cave(std::vector<RockBoundary>& rockBoundaries, const std::pair<int, int>& hole, bool isPart2=false);

        void addRocksBetween(const std::pair<int, int> &rock1, const std::pair<int, int> &rock2);
        void fillWithRocks(const std::vector<RockBoundary>& rockBoundaries);
        bool addSand();
        const std::vector<char>& operator[](size_t row) const;
        friend std::ostream& operator<<(std::ostream& out, const Cave& cave);


        size_t getYmax() const;
        size_t getXmax() const;
    };

    class Sand
    {
    private:
        int m_x{};
        int m_y{};
    public:
        explicit Sand(std::pair<int, int> hole) : m_x{hole.first}, m_y{hole.second}
        {}

        enum GoResult
        {
            successful,
            blocked,
            outOfRange
        };

        int getX() const { return m_x; }
        int getY() const { return m_y; }

        GoResult goDown(const Cave& cave);
        GoResult goLeftDown(const Cave& cave);
        GoResult goRightDown(const Cave& cave);

    };
}

#endif //ADVENT_OF_CODE_2022_AOC14_H
