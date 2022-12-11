#include "aoc1.h"

namespace aoc1
{
    class Elf
    {
    private:
        std::vector<int> m_calories{ };

    public:
        explicit Elf(std::vector<int> &calories) : m_calories{ calories }
        {

        }

        int getSumCalorie()
        { return std::reduce(m_calories.begin(), m_calories.end()); }

        bool hasMoreCalories(Elf &otherElf)
        { return getSumCalorie() > otherElf.getSumCalorie(); };
    };

    void run()
    {
        // create elves with calories carried
        std::vector<Elf> elves{ };
        {
            std::vector<int> calories;
            for (const auto &str: helper::readInput("../input/aoc1.txt"))
            {
                if (str.empty())
                {
                    elves.emplace_back(calories);
                    calories.clear();
                } else
                {
                    calories.push_back(std::stoi(str));
                }
            }
        }

        // sort elves based on max calory carries
        std::sort(elves.begin(), elves.end(), [](auto &elf1, auto &elf2) {
            return elf1.hasMoreCalories(elf2);
        });

        // print max calories
        int sum{ };
        for (size_t i{ 0 }; i < 3; i++)
        {
            int sumCalories{ elves[i].getSumCalorie() };
            std::cout << sumCalories << '\n';
            sum += sumCalories;
        }

        std::cout << "Sum of calories: " << sum << '\n';
    }
}


