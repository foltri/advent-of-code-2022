//
// Created by Balint Zsiga on 2022. 12. 06..
//

#include "aoc5.h"


namespace aoc5
{
    struct Step
    {
        int amount{};
        int from{};
        int to{};
    };

    std::vector<Step> getSteps(const std::vector<std::string>& data, const size_t numSteps)
    {
        std::vector<Step> steps{};
        steps.reserve(numSteps);

        size_t stepsStartIndex{ data.size() - numSteps };

        for (size_t i{stepsStartIndex}; i<data.size();++i)
        {
            //move 1 from 2 to 1
            auto fIndex{ data[i].find('f') };
            auto tIndex{ data[i].find('t') };

            int amount  { std::stoi(data[i].substr(5, fIndex)) };
            int from    { std::stoi(data[i].substr(fIndex+5, tIndex)) };
            int to      { std::stoi(data[i].substr(tIndex+3)) };

            steps.push_back({amount, from, to});
        }
        return steps;
    }

    using Stacks = std::map<size_t , std::string>;

    Stacks getStacks(const std::vector<std::string>& data, const size_t stackNumIndex)
    {
        Stacks stacks{};

        for (size_t lineIndex{0}; lineIndex < stackNumIndex; ++lineIndex)
        {
            size_t charIndex{ 1 };

            while (charIndex < data[lineIndex].length())
            {
                char potentialCargo{ data[lineIndex][charIndex] };
                if (potentialCargo != ' ')
                {
                    auto stack {stacks.find(static_cast<size_t>(data[stackNumIndex][charIndex] - 48))};   // this only works with single digit stack numbers
                    if (stack != stacks.end()) stack->second.insert(0, 1, potentialCargo);
                    else
                    {
                        stacks.insert({static_cast<size_t>(data[stackNumIndex][charIndex] - 48), std::string{potentialCargo}});
                    }
                }
                charIndex += 4;
            }
        }
        return stacks;
    }

    void executeStep(Stacks& stacks, Step step)
    {
        for (int i{0}; i < step.amount; ++i)
        {
            stacks[static_cast<size_t>(step.to)].push_back(stacks[static_cast<size_t>(step.from)].back());
            stacks[static_cast<size_t>(step.from)].pop_back();
        }
    }

    void executeStepNew(Stacks& stacks, Step step)
    {
        std::string cratesToMove { stacks[static_cast<size_t>(step.from)].substr(stacks[static_cast<size_t>(step.from)].length()-static_cast<size_t>(step.amount), static_cast<size_t>(step.amount)) };
        stacks[static_cast<size_t>(step.to)].append(cratesToMove);
        for (int i{0}; i < step.amount; ++i)
        {
            stacks[static_cast<size_t>(step.from)].pop_back();
        }
    }

    std::string getTopCrates(const Stacks& stacks)
    {
        std::string topCrates{};
        for (auto& stack : stacks)
        {
            topCrates.push_back(stack.second.back());
        }

        return topCrates;
    }

    void run()
    {
        auto data{ helper::readInput("../input/aoc5.txt") };

        // get the start indices of interesting parts in the data
        auto line{ data.rbegin() };
        size_t numSteps{0};
        while(!(*line++).empty()) { numSteps++; };
        const size_t stepsStartIndex { data.size() - numSteps };
        const size_t stackNumIndex { stepsStartIndex - 2 };

        // get parsed steps
        auto steps { getSteps(data, numSteps) };
        // get parsed stacks
        auto stacks{ getStacks(data, stackNumIndex) };

        for (auto& step : steps)
        {
            executeStepNew(stacks, step);
        }

        std::cout << "The top crates are " << getTopCrates(stacks);
    }
}