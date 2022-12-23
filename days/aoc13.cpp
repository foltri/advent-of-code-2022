//
// Created by Balint Zsiga on 2022. 12. 23..
//

#include "aoc13.h"

namespace aoc13
{
    void addBracketClosing(std::stringstream& ss)
    {
        auto prevTellg { ss.tellg() };

        // extract number
        int number{};
        ss >> number;

        // add ']' after the number
        auto tmp { ss.str() };
        tmp.insert(ss.tellg(), "]");
        ss.str(tmp);

        // restore the input position indicator before the number
        ss.seekg(prevTellg);
    }

    bool isRightOrder(std::string_view swL, std::string_view swR)
    {
        // making a copy so that inputs are not manipulated
        std::stringstream strL{swL.data()};
        std::stringstream strR{swR.data()};

        // ignore first '['
        strL.ignore();
        strR.ignore();

        while (true)
        {
            auto l{ strL.peek() };
            auto r{ strR.peek() };

            if (l == -1)
            {
//                std::cout << "In the right order: left went out of all values at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                return true;
            }

            if (r == -1)
            {
//                std::cout << "Not in the right order: right went out of all values at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                return false;
            }

            // ignore commas
            if (l == ',' || l == ' ')
            {
                strL.ignore();
            }
            if (r == ',' || r == ' ')
            {
                strR.ignore();
            }

            else if ((l == '[' && r == '[') || (l == ']' && r == ']'))
            {
                strL.ignore();
                strR.ignore();
            }

            else if (l == '[' && r == ']')
            {
//                std::cout << "Not in the right order: right went out list values at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                return false;
            }

            else if (l == ']' && r == '[')
            {
//                std::cout << "In the right order: left went out list values at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                return true;
            }

            else if (l == '[') // right is a number
            {
                // add brackets around rightNumber (only ])
                addBracketClosing(strR);
                strL.ignore();  // ignore '[' in left, '[' of the new bracket is also ignored in right (not added)
            }

            else if (l == ']') // right is a number
            {
//                std::cout << "In the right order: left went out list values at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                return true;
            }

            else    // left is a number
            {
                if (r == '[')
                {
                    // add brackets around leftNumber (only ])
                    addBracketClosing(strL);
                    strR.ignore();  // ignore '[' in right, '[' of the new bracket is also ignored in left (not added)
                }

                else if (r == ']')
                {
//                    std::cout << "Not in the right order: right went out list values at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                    return false;
                }

                else // both left and right are numbers
                {
//                    auto lPrevTellg { strL.tellg() };
//                    auto rPrevTellg { strR.tellg() };

                    int leftNumber{};
                    strL >> leftNumber;

                    int rightNumber{};
                    strR >> rightNumber;

                    if (leftNumber < rightNumber)
                    {
//                        strL.seekg( lPrevTellg );
//                        strR.seekg( rPrevTellg );
//                        std::cout << "In the right order: left number < right number at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                        return true;
                    }
                    else if (leftNumber > rightNumber)
                    {
//                        strL.seekg( lPrevTellg );
//                        strR.seekg( rPrevTellg );
//                        std::cout << "Not in the right order: left number > right number at \n" << strL.rdbuf() << '\n' << strR.rdbuf() << "\n\n";
                        return false;
                    }
                }
            }
        }
    }

    void part1()
    {
        auto input {helper::readInput("../input/aoc13.txt")};
        std::vector<bool> isRightOrders{};
        for (size_t i{0}; i<input.size(); i+=3)
        {
            isRightOrders.push_back(isRightOrder(input[i], input[i+1]));
        }

        // get the sum of indices
        size_t sumOfIndices{0};
        for (size_t i{0}; i < isRightOrders.size(); ++i)
        {
            if (isRightOrders[i]) sumOfIndices+=(i+1);
        }

        std::cout << "The sum of the indices of the packets in right order is " << sumOfIndices << '\n';
    }

    void part2()
    {
        auto input {helper::readInput("../input/aoc13.txt")};

        size_t d1Index{1};
        size_t d2Index{2};  // starts from 2 as divider 1 is also in front of it

        const std::string divider1{ "[[2]]" };
        const std::string divider2{ "[[6]]" };

        for (size_t i{0}; i<input.size(); ++i)
        {
            if (!input[i].empty())
            {
                if (isRightOrder(input[i], divider1))
                {
                    ++d1Index;
                    ++d2Index;
                }
                else if (isRightOrder(input[i], divider2))
                {
                    ++d2Index;
                }
            }
        }

        std::cout << "d1Index: " << d1Index << "\nd2Index: " << d2Index << "\ndecoder key: " << d1Index*d2Index << '\n';
    }

    void run()
    {
        part1();
        part2();
    }
}