//
// Created by Balint Zsiga on 2022. 12. 03..
//

//
// Created by Balint Zsiga on 2022. 12. 03..
//

#include "aoc2b.h"

namespace aoc2b
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
        Shape m_myShape{ };
        const Outcome m_outcome{ };

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

        static Shape getStrongerShape(Shape shape)
        {
            static const std::map<Shape, Shape> defeatingTable
                    {
                            { Shape::scissor, Shape::rock },
                            { Shape::rock,    Shape::paper },
                            { Shape::paper,   Shape::scissor },
                    };

            return defeatingTable.at(shape);
        }

        Shape getMyShape()
        {
            switch (m_outcome)
            {
                case Outcome::draw:
                    return m_elfShape;
                case Outcome::lost:
                    return getWeakerShape(m_elfShape);
                case Outcome::won:
                    return getStrongerShape(m_elfShape);
            }
        }

    public:
        Round(Shape elfShape, Outcome outcome) : m_elfShape{ elfShape }, m_outcome{ outcome }
        {
            m_myShape = getMyShape();
        }

        int getScore()
        {
            return static_cast<int>(m_outcome) + static_cast<int>(m_myShape);
        }
    };

    void run()
    {
        const std::map<char, Shape> shapeDict
                {
                        { 'A', Shape::rock },
                        { 'B', Shape::paper },
                        { 'C', Shape::scissor },
                };

        const std::map<char, Outcome> outcomeDict
                {
                        { 'X', Outcome::lost },
                        { 'Y', Outcome::draw },
                        { 'Z', Outcome::won },
                };

        std::vector<Round> rounds{ };
        for (auto &str: helper::readInput("../input/aoc2.txt"))
        {
            if (!str.empty())
                rounds.emplace_back(shapeDict.at(str[0]), outcomeDict.at(str[2]));
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

