//
// Created by Balint Zsiga on 2022. 12. 03..
//

#include "aoc2.h"

namespace aoc2
{
    enum class Shape
    {
        rock = 1,
        paper,
        scissor,
    };

    enum class Outcome
    {
        lost = 0,
        draw = 3,
        won = 6,
    };

    class Round
    {
    private:
        const Shape m_elfShape{ };
        const Shape m_myShape{ };
        Outcome m_outcome{ };

        static Shape getWeakerShape(Shape shape)
        {
            static const std::map<Shape, Shape> defeatTable
                    {
                            { Shape::rock,    Shape::scissor },
                            { Shape::paper,   Shape::rock },
                            { Shape::scissor, Shape::paper },
                    };

            return defeatTable.at(shape);
        }

        Outcome getOutcome()
        {
            if (m_myShape == m_elfShape) return Outcome::draw;
            if (getWeakerShape(m_myShape) == m_elfShape) return Outcome::won;

            return Outcome::lost;
        }

    public:
        Round(Shape elfShape, Shape myShape) : m_elfShape{ elfShape }, m_myShape{ myShape }
        {
            m_outcome = getOutcome();
        }

        int getScore()
        {
            return static_cast<int>(getOutcome()) + static_cast<int>(m_myShape);
        }
    };

    void run()
    {
        const std::map<char, Shape> shapeDict
                {
                        { 'A', Shape::rock },
                        { 'B', Shape::paper },
                        { 'C', Shape::scissor },
                        { 'X', Shape::rock },
                        { 'Y', Shape::paper },
                        { 'Z', Shape::scissor },
                };

        std::vector<Round> rounds{ };
        for (auto &str: helper::readInput("../input/aoc2.txt_test"))
        {
            if (!str.empty())
                rounds.emplace_back(shapeDict.at(str[0]), shapeDict.at(str[2]));
        }

        // get total score
        int totalScore{ };
        for (auto &round: rounds)
        {
            totalScore += round.getScore();
        }

        std::cout << "The total score is: " << totalScore << '\n';
    }
}
