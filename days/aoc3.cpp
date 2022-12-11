//
// Created by Balint Zsiga on 2022. 12. 04..
//

#include "aoc3.h"

namespace  aoc3
{
    int getPriority(char item)
    {
        assert(item <= 122 && item >= 65 && (item <= 90 || item >= 97) && "Unknown item.");

        // lower case
        if (item > 90) return item - 96;
        else return item - 38;
    }

    using Items = std::set<char>;
    using ItemsMulti = std::multiset<char>;

    class Rucksack
    {
    private:
        Items m_comp1Items{ };
        Items m_comp2Items{ };

    public:
        explicit Rucksack(std::string_view items)
        {
            size_t count{ 1 };
            size_t half{ items.length() / 2 };
            for (auto item: items)
            {
                if (count <= half) m_comp1Items.insert(item);
                else m_comp2Items.insert(item);
                count++;
            }
        }

        Items getComp1Items()
        { return m_comp1Items; };

        Items getComp2Items()
        { return m_comp2Items; };

        Items getAllItems()
        {
            Items all{ };
            std::set_union(m_comp1Items.begin(), m_comp1Items.end(), m_comp2Items.begin(), m_comp2Items.end(),
                           std::inserter(all, all.end()));
            return all;
        }
    };

// template so it works with both sets and multisets
    template<typename T>
    void getMatches(const Items &i1, const T &i2, ItemsMulti &matches)
    {
        std::set_intersection(i1.begin(), i1.end(), i2.begin(), i2.end(), std::inserter(matches, matches.end()));
    }

    int getSumPriorities(ItemsMulti &items)
    {
        int sum{ 0 };
        for (auto item: items)
        {
            sum += getPriority(item);
        }
        return sum;
    }

    void run()
    {
        // fill the rucksacks
        std::vector<Rucksack> rucksacks{ };
        {
            for (std::string_view items: helper::readInput("../input/aoc3.txt"))
            {
                assert((items.length() % 2 == 0) && "Uneven number of items.");

                rucksacks.emplace_back(items);
            }

            // find the errors in each rucksack
            ItemsMulti errorItems{ };
            for (auto &rucksack: rucksacks)
                getMatches(rucksack.getComp1Items(), rucksack.getComp2Items(), errorItems);

            // calculate the sum of error item priorities
            std::cout << "The sum of the error item priorities is " << getSumPriorities(errorItems) << '\n';
        }

        // PART 2
        ItemsMulti badgeItems{ };
        for (size_t i{ 2 }; i < rucksacks.size(); i += 3)
        {
            ItemsMulti tmpMatches{ };
            getMatches(rucksacks[i - 2].getAllItems(), rucksacks[i - 1].getAllItems(), tmpMatches);
            getMatches(rucksacks[i].getAllItems(), tmpMatches, badgeItems);
        }

        std::cout << "The sum of the badge item priorities is " << getSumPriorities(badgeItems) << '\n';

    }
}
