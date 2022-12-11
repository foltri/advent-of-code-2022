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
    Knot m_head{};
    Knot m_tail{};
    std::set<Knot> m_uniqueTailPositions{ Knot{ 0, 0}};

    int getXDiff() const { return m_head.x - m_tail.x; }
    int getYDiff() const { return m_head.y - m_tail.y; };

    void moveTail()
    {
        if (std::abs(getXDiff()) > 1 || std::abs(getYDiff()) > 1)
        {
            do
            {
                int xDiff { getXDiff() };
                int yDiff { getYDiff() };
                if (std::abs(xDiff) > 0) m_tail.move(xDiff / std::abs(xDiff), 0);
                if (std::abs(yDiff) > 0) m_tail.move(0, yDiff / std::abs(yDiff));
            } while ((getXDiff() != 0) == (getYDiff() != 0));

            m_uniqueTailPositions.insert(m_tail);
        }
    }

public:
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
            m_head.move(x, y);
            moveTail();
        }
    }

    size_t getNumOfUniqueTailPositions() const { return m_uniqueTailPositions.size(); }
};

namespace aoc9
{
    void run()
    {
        Rope rope{};
        for (const std::string& command : helper::readInput("../input/aoc9.txt"))
        {
            rope.move(command[0], std::stoi(command.substr(command.find(' '))));
        }

        std::cout << "The number of unique m_tail positions is: " << rope.getNumOfUniqueTailPositions() << '\n';
    }
}
