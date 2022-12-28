//
// Created by Balint Zsiga on 2022. 12. 24..
//

#include "aoc14.h"

namespace aoc14
{
    Cave::Cave(std::vector<RockBoundary>& rockBoundaries, const std::pair<int, int>& hole, bool isPart2) : m_hole{ hole }
    {
        // get max x and y
        auto xMax { std::max_element(rockBoundaries.begin(), rockBoundaries.end(), [](const auto& a, const auto& b) { return a.position.first < b.position.first; })->position.first };
        auto yMax { std::max_element(rockBoundaries.begin(), rockBoundaries.end(), [](const auto& a, const auto& b) { return a.position.second < b.position.second; })->position.second };

        // make wider cave for part 2 - came ou with 6* by trial and error
        auto xLen{ isPart2 ? (xMax + 1) * 6 : (xMax + 1)};
        auto yLen{ isPart2 ? yMax + 3 : yMax + 1 };

        // init cave with air ('.')
        // +2 for the ground
        m_data.reserve(yLen);
        for (size_t i{0}; i<m_data.capacity(); ++i) m_data.emplace_back(xLen, '.');

        if (isPart2)
        {
            // adjust rockboundaries and hole to wider cave
            auto adjust{ (xLen / 2) - xMax };
            for (auto &rockBoundary: rockBoundaries)
            {
                rockBoundary.position.first += adjust;
            }
            m_hole.first += adjust;

            // fill last row with rocks
            m_data.back() = std::vector<char> (xLen, '#');
        }
    }

    void Cave::addRocksBetween(const std::pair<int, int>& rock1, const std::pair<int, int>& rock2)
    {
        // rock - first = x - second = y
        // cave [y][x]
        if (rock1.first == rock2.first)
        {
            int min {std::min(rock1.second, rock2.second)};
            int max {std::max(rock1.second, rock2.second)};
            for (int y{min}; y<=max; ++y) m_data[y][rock1.first] = '#';
        }
        else
        {
            int min {std::min(rock1.first, rock2.first)};
            int max {std::max(rock1.first, rock2.first)};
            for (int x{min}; x<=max; ++x) m_data[rock1.second][x] = '#';
        }
    }

    void Cave::fillWithRocks(const std::vector<RockBoundary>& rockBoundaries)
    {
        // add first rock
        auto prevBoundary { rockBoundaries.front() };

        for (size_t i{1}; i < rockBoundaries.size(); ++i)
        {
            auto boundary {rockBoundaries[i]};
            if (prevBoundary.index == boundary.index)
            {
                addRocksBetween(prevBoundary.position, boundary.position);
            }
            prevBoundary = boundary;
        }
    }

    bool Cave::addSand()
    {
        Sand sand { m_hole };
        while (true)
        {
            Sand::GoResult res{};
            res = sand.goDown(*this);
            if (res == Sand::GoResult::successful) continue;
            else if (res == Sand::GoResult::outOfRange)
                return false;

            res = sand.goLeftDown(*this);
            if (res == Sand::GoResult::successful) continue;
            else if (res == Sand::GoResult::outOfRange)
                return false;

            res = sand.goRightDown(*this);
            if (res == Sand::GoResult::successful) continue;
            else if (res == Sand::GoResult::outOfRange)
            {
                std::cout << *this;
                return false;
            }

            // sand is blocked in every direction if we get here
            m_data[sand.getY()][sand.getX()] = 'o';

            // send is blocking the hole
            if (sand.getY() == m_hole.second && sand.getY() == m_hole.second)
                return false;

            return true;
        }
    }

    const std::vector<char>& Cave::operator[](size_t row) const
    {
        return m_data[row];
    }

    std::ostream& operator<<(std::ostream& out, const Cave& cave)
    {
        for (auto& line : cave.m_data)
        {
            for (auto c : line)
            {
                out << c << ' ';
            }
            out << '\n';
        }
        return out;
    }

    size_t Cave::getYmax() const { return m_data.size()-1; }
    size_t Cave::getXmax() const { return m_data[0].size()-1; }


    Sand::GoResult Sand::goDown(const Cave& cave)
    {
        if (m_y+1 > cave.getYmax()) return Sand::GoResult::outOfRange;

        if (cave[m_y+1][m_x] == '.')
        {
            m_y += 1;
            return Sand::GoResult::successful;
        }

        return Sand::GoResult::blocked;
    }

    Sand::GoResult Sand::goLeftDown(const Cave& cave)
    {
        if (m_y+1 > cave.getYmax()) return Sand::GoResult::outOfRange;
        if (m_x-1 < 0) return Sand::GoResult::outOfRange;

        if (cave[m_y+1][m_x-1] == '.')
        {
            m_x -= 1;
            m_y += 1;
            return Sand::GoResult::successful;
        }

        return Sand::GoResult::blocked;
    }

    Sand::GoResult Sand::goRightDown(const Cave& cave)
    {
        if (m_y+1 > cave.getYmax()) return Sand::GoResult::outOfRange;
        if (m_x+1 > cave.getXmax()) return Sand::GoResult::outOfRange;

        if (cave[m_y+1][m_x+1] == '.')
        {
            m_x += 1;
            m_y += 1;
            return Sand::GoResult::successful;
        }

        return Sand::GoResult::blocked;
    }

    void simplify(std::vector<RockBoundary>& rockBoundaries)
    {
        // get min x and y
        auto xMin { std::min_element(rockBoundaries.begin(), rockBoundaries.end(), [](const auto& a, const auto& b) { return a.position.first < b.position.first; })->position.first };
        auto yMin { std::min_element(rockBoundaries.begin(), rockBoundaries.end(), [](const auto& a, const auto& b) { return a.position.second < b.position.second; })->position.second };

        // simplify rock coordinates
        std::transform(rockBoundaries.begin(), rockBoundaries.end(), rockBoundaries.begin(), [xMin, yMin](auto& rock) { rock.position.first -= xMin; rock.position.second -= yMin; return rock;});
    }

    std::vector<RockBoundary> getRockBoundaries(const std::vector<std::string>& input)
    {
        std::vector<RockBoundary> rockBoundaries{};
        int index{0};
        for (auto& line : input)
        {
            std::stringstream sLine{ line };
            while(sLine.peek() != -1)
            {
                switch (sLine.peek())
                {
                    case '-':
                    case '>':
                    case ' ':
                        sLine.ignore();
                        break;
                    default:
                        int x{};
                        sLine >> x;

                        sLine.ignore(); // ignore the ','

                        int y{};
                        sLine >> y;

                        rockBoundaries.emplace_back(RockBoundary{{ x, y}, index});
                        break;
                }
            }
            ++index;
        }
        return rockBoundaries;
    }

    void run()
    {
        constexpr bool isPart2{ true };

        std::pair<int, int> hole {500,0};

        auto input { helper::readInput("../test_input/aoc14.txt") };
        auto rockBoundaries { getRockBoundaries(input) };

        // add hole for the right cave size
        rockBoundaries.push_back(RockBoundary{hole,-1});
        simplify(rockBoundaries);

        // update to simplified hole and remove from rockBoundaries
        hole = rockBoundaries.back().position;
        rockBoundaries.pop_back();

        Cave cave{ rockBoundaries, hole, isPart2 };
        cave.fillWithRocks(rockBoundaries);

        int count{ isPart2 ? 1 : 0};
        while (cave.addSand())
        {
            ++count;
        }

        //std::cout << cave;    // show cave
        std::cout << "answer: " << count;
    }


}
