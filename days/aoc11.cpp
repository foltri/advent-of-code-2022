//
// Created by Balint Zsiga on 2022. 12. 12..
//

#include "aoc11.h"

namespace aoc11
{
    using Item = unsigned long;
    using MonkeyAndItem = std::pair<size_t , Item>;

    class Monkey
    {
    private:
        std::deque<Item>m_items{};
        std::function<const MonkeyAndItem&(Item)> m_monkeyAndItemFunc;
        unsigned long m_numInspections{0};

        static constexpr unsigned int s_worryLevelDivisor{ 1 };
        static unsigned int s_LCD;  // least common divisor to divide items to avoid overflow
        static std::set<unsigned int> s_divisors;   // save all divisors for calculating LCD

        void parseMonkeyInput(const std::deque<std::string>& data)
        {
            const bool isOld{ data[2][25] == 'o' };
            int secondOperand{};
            if (!isOld) secondOperand = std::stoi(data[2].substr(25));

            const bool isAddition{ data[2][23] == '+' };
            const unsigned int divisor{ static_cast<unsigned int>(std::stoi(data[3].substr(21))) };
            const size_t trueMonkey{ static_cast<size_t>(std::stoi(data[4].substr(29))) };
            const size_t falseMonkey{ static_cast<size_t>(std::stoi(data[5].substr(30))) };

            s_addDivisor(divisor);
            auto getMonkeyAndItem { [=](Item newItem) { return (newItem % divisor == 0) ? MonkeyAndItem{ trueMonkey, newItem } : MonkeyAndItem{ falseMonkey, newItem }; }};

            if (isOld)
            {
                if (isAddition) m_monkeyAndItemFunc = [=](Item item) {
                        assert((item < ULLONG_MAX - item) && "newItem would overflow");
                        Item newItem{ ((item + item) / s_worryLevelDivisor) % s_LCD };
                        return getMonkeyAndItem(newItem);
                    };
                else m_monkeyAndItemFunc = [=](Item item) {
                        assert((item < ULLONG_MAX / item) && "newItem would overflow");
                        Item newItem{ (item * item / s_worryLevelDivisor) % s_LCD };
                        return getMonkeyAndItem(newItem);
                    };
            }
            else
            {
                if (isAddition) m_monkeyAndItemFunc = [=](Item item) {
                        assert((item < ULLONG_MAX - secondOperand) && "newItem would overflow");
                        Item newItem{ ((item + secondOperand) / s_worryLevelDivisor) % s_LCD };
                        return getMonkeyAndItem(newItem);
                    };
                else m_monkeyAndItemFunc = [=](Item item) {
                        assert((item < ULLONG_MAX / secondOperand) && "newItem would overflow");
                        Item newItem{ ((item * secondOperand) / s_worryLevelDivisor) % s_LCD };
                        return getMonkeyAndItem(newItem);
                    };
            }

            // parse items
            std::stringstream lineStream{ data[1].substr(18) };
            for (int item; lineStream >> item;) {
                lineStream.ignore(2);
                m_items.push_back(item);
            }
        }

    public:
        Monkey(const std::deque<std::string>& data)
        {
            parseMonkeyInput(data);
        }

        void addItem(Item item) { m_items.push_back(item); }

        size_t getNumItems() const { return m_items.size(); }
        size_t getNumInspections() const { return m_numInspections; }

        MonkeyAndItem getPassedItem()
        {
            if (!m_items.empty())
            {
                ++m_numInspections;
                Item item{ m_items.front() };
                m_items.pop_front();
                return m_monkeyAndItemFunc(item);
            }
            else return {-1, -1};
        }

        static void s_addDivisor(unsigned int divisor)
        {
            // if the new divisor is unique
            // based on the input all of them are primes and unique that's why the simplified LCD calculation
            if (s_divisors.insert(divisor).second) s_LCD *= divisor;
        }

    };

    // init statics
    unsigned int Monkey::s_LCD{1};
    std::set<unsigned int> Monkey::s_divisors{};

    void run()
    {
        constexpr int lenMonkeyData{6};
        auto data { helper::readInput("../input/aoc11.txt") };
        std::deque<Monkey> monkeys{};

        // create monkeys
        for (int startI{0}; startI<=data.size()-lenMonkeyData; startI+=lenMonkeyData+1)
        {
            std::deque<std::string> dataSlice {data.begin()+startI, data.begin()+startI+lenMonkeyData};
            monkeys.emplace_back(dataSlice);
        }

        int r{-1};
        while(++r<10000)
        {
            for (auto &monkey: monkeys)
            {
                while (monkey.getNumItems())
                {
                    auto monkeyAndItem{ monkey.getPassedItem() };
                    monkeys[monkeyAndItem.first].addItem(monkeyAndItem.second);
                }
            }
        }

        // get the most active monkeys
        std::vector<size_t> numInspections{};
        for (auto& monkey : monkeys)
        {
            numInspections.push_back(monkey.getNumInspections());
        }
        std::sort(numInspections.begin(), numInspections.end(), std::greater<>());

        std::cout << "The level of monkey business is: " << numInspections[0] * numInspections[1] << '\n';
    }
}