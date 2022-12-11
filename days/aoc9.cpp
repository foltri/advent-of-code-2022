//
// Created by Balint Zsiga on 2022. 12. 11..
//

#include "aoc9.h"

struct Knot
{
    int x{0};
    int y{0};

    void move(int xPos, int yPos)
    {
        x+=xPos;
        y+=yPos;
    }

    bool operator<(const Knot& k) const
    {
        return (x < k.x) || ((x == k.x) && (y < k.y));
    }
};

class Rope
{
private:
    std::vector<Knot> m_knots{};
    std::set<Knot> m_uniqueTailPositions{ Knot{ 0, 0}};

    int getXDiff(size_t i) const { return m_knots[i-1].x - m_knots[i].x; }
    int getYDiff(size_t i) const { return m_knots[i-1].y - m_knots[i].y; };

    void moveTails()
    {
        for (size_t i{1}; i < m_knots.size(); ++i)
        {
            if (std::abs(getXDiff(i)) > 1 || std::abs(getYDiff(i)) > 1)
            {
                do
                {
                    int xDiff{ getXDiff(i) };
                    int yDiff{ getYDiff(i) };
                    if (std::abs(xDiff) > 0) m_knots[i].move(xDiff / std::abs(xDiff), 0);
                    if (std::abs(yDiff) > 0) m_knots[i].move(0, yDiff / std::abs(yDiff));
                } while ((getXDiff(i) + getYDiff(i)) > 2);

                // add new tail (last knot) position
                if (i == m_knots.size()-1) m_uniqueTailPositions.insert(m_knots[i]);
            }
        }
    }

public:
    Rope() = delete;

    explicit Rope(size_t numKnots)
    {
        m_knots.reserve(numKnots);
        for (size_t i{0}; i<numKnots; ++i) m_knots.push_back(Knot{});
    }

    void move(char dir, int value)
    {
        int x{0};
        int y{0};
        switch (dir)
        {
            case 'L': x-=1; break;
            case 'R': x+=1; break;
            case 'U': y+=1; break;
            case 'D': y-=1; break;
            default: std::cerr << "Unknown direction: " << dir; break;
        }

        for (int count{0}; count < value; ++count)
        {
            // move the head (first knot)
            m_knots[0].move(x, y);
            moveTails();
        }
    }

    size_t getNumOfUniqueTailPositions() const { return m_uniqueTailPositions.size(); }
};

namespace aoc9
{
    void run()
    {
        Rope rope{10};
        for (const std::string& command : helper::readInput("../input/aoc9.txt"))
        {
            rope.move(command[0], std::stoi(command.substr(command.find(' '))));
        }

        std::cout << "The number of unique m_tail positions is: " << rope.getNumOfUniqueTailPositions() << '\n';
    }
}
