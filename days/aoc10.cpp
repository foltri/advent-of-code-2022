//
// Created by Balint Zsiga on 2022. 12. 12..
//

#include "aoc10.h"

namespace aoc10
{
    void run()
    {
        constexpr std::array<size_t, 6> multipliers{20, 60, 100, 140, 180, 220};
        std::array<int, 6> signalStrength{};

        const auto commands { helper::readInput("../input/aoc10.txt")};

        size_t iMultipliers{0};
        size_t iCycle {0};
        int xReg{1};
        for (size_t iCommand{0}; iCommand < commands.size(); ++iCommand)
        {
            // get command length from command type
            int commandLength{1};
            if (commands[iCommand][0] == 'a') commandLength=2;

            // check multipliers every iCycle (addx is 2 cycles, npp is one)
            for (int iCc{ 1}; iCc <= commandLength; ++iCc)
            {
                if (multipliers[iMultipliers] == ++iCycle)
                {
                    signalStrength[iMultipliers] = static_cast<int>(iCycle) * xReg;
                    ++iMultipliers;
                }

                // print
                int crtPos{ (static_cast<int>(iCycle)-1) % 40 };
                if (crtPos == 0) std::cout << '\n';
                if (crtPos >= xReg-1 && crtPos <= xReg+1) std::cout << "# ";
                else std::cout << ". ";
            }
            
            // calc xReg
            if (commands[iCommand][0] == 'a') xReg += std::stoi(commands[iCommand].substr(4 ));
        }

        int sum { std::reduce(signalStrength.begin(), signalStrength.end()) };

        std::cout << "\nThe sum of the 6 signal strength is: " << sum  << '\n';
    }
}
