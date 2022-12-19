//
// Created by Balint Zsiga on 2022. 12. 15..
//

#include "aoc12.h"

namespace aoc12
{
    class AdjList
    {
    private:
        std::vector<std::vector<size_t>>nodes{};
    public:
        
        explicit AdjList(size_t size) : nodes{ std::vector<std::vector<size_t>>(size) }
        {

        }
        
        void setEdge(size_t n1, size_t n2)
        {
            nodes[n1].push_back(n2);
        }

        void setEdgeMultiDir(size_t n1, size_t n2)
        {
            nodes[n1].push_back(n2);
            nodes[n2].push_back(n1);
        }

        const std::vector<size_t>& operator[](size_t i)
        {
            return nodes[i];
        }

        size_t getNumNodes() const { return nodes.size(); }
        
    };

    // for debugging
    void getRouteTo(const std::vector<size_t>& from, size_t node, std::vector<size_t>& route)
    {
        while (from[node] != node)
        {
            node = from[node];
            route.push_back(node);
        }
    }

    size_t getShortestDistNode(const std::vector<size_t>& dists, const std::vector<bool>& isFinal)
    {
        size_t min {INT_MAX};
        size_t indexOfMin{0};
        for (size_t i{0}; i < dists.size(); ++i)
        {
            if (!isFinal[i] && dists[i] < min)
            {
                min = dists[i];
                indexOfMin = i;
            }
        }
        return indexOfMin;
    }

    std::vector<size_t> getShortestDists(AdjList& adjList, size_t start)
    {
        std::vector<bool> isFinal(adjList.getNumNodes(), false);
        std::vector<size_t> shortestDists(adjList.getNumNodes(), INT_MAX);
        std::vector<size_t> from(adjList.getNumNodes());
        shortestDists[start] = 0; // distance of start is 0

        for (size_t i{0}; i < shortestDists.size(); ++i)
        {
            size_t shortestDistNode{ getShortestDistNode(shortestDists, isFinal) };
            isFinal[shortestDistNode] = true;

            // update shortestDists with neighbours of current shortestDistNode
            for (auto node : adjList[shortestDistNode])
            {
                size_t& r_olDist { shortestDists[node] };
                size_t newDist { shortestDists[shortestDistNode] + 1 };
                if (!isFinal[node] && r_olDist > newDist )
                {
                    r_olDist = newDist;
                    from[node] = shortestDistNode;
                }
            }
        }

        return shortestDists;
    }

    void run()
    {
        // set this to change between part1 or part2 answer
        constexpr bool isPart2{ true };

        auto data { helper::readInput("../input/aoc12.txt") };
        AdjList adjList{ data.size() * data[0].length() };

        size_t start{};
        size_t destination{};
        size_t index{0};
        std::vector<size_t> aIndexes{};

        // init AdjList
        for (size_t row{0}; row < data.size(); ++row)
        {
            for (size_t col{0}; col < data[0].length(); ++col)
            {
                const size_t indexRight { index+1 };
                const size_t indexBelow { index + data[0].length() };

                // save start and destination and overwrite their height
                // this is not too efficient as we check almost every cell 3 times
                if (data[row][col] == 'S') { start = index; data[row][col] = 'a'; }
                else if (data[row][col] == 'E') { destination = index; data[row][col] = 'z'; }
                if (data[row][col+1] == 'S') { start = indexRight; data[row][col+1] = 'a'; }
                else if (data[row][col+1] == 'E') { destination = indexRight; data[row][col+1] = 'z'; }
                if (data[row+1][col] == 'S') { start = indexBelow; data[row+1][col] = 'a'; }
                else if (data[row+1][col] == 'E') { destination = indexBelow; data[row+1][col] = 'z'; }

                char current{ data[row][col] };
                const char right{ data[row][col+1] };
                const char below{ data[row+1][col] };

                // save 'a' indexes for part2
                if (current == 'a') aIndexes.push_back(index);

                // check the square to the right
                // don't check squares to the right of the last column
                if (col < data[row].length()-1)
                {
                    const int dist { current - right };
                    if (std::abs(dist) < 2)         adjList.setEdgeMultiDir(index, indexRight);
                    else if ((dist > 0) != isPart2) adjList.setEdge(index, indexRight);
                    else                            adjList.setEdge(indexRight, index);

                }

                // check the square below
                // don't check squares under the last row
                if (row < data.size()-1)
                {
                    const int dist { current - below };
                    if (std::abs(dist) < 2)         adjList.setEdgeMultiDir(index, indexBelow);
                    else if ((dist > 0) != isPart2) adjList.setEdge(index, indexBelow);
                    else                            adjList.setEdge(indexBelow, index);
                }
                ++index;
            }
        }

        // part1 answer
        if (!isPart2)
        {
            auto shortestDist{ getShortestDists(adjList, start)[destination] };
            std::cout << "The shortest dist from " << start << " to " << destination << " is " << shortestDist << ".\n";
        }
        // part2 answer
        else
        {
            auto distsFromDest{ getShortestDists(adjList, destination) };
            std::vector<size_t> aDists{ };
            for (auto i: aIndexes) aDists.push_back(distsFromDest[i]);
            std::sort(aDists.begin(), aDists.end());
            std::cout << "The shortest 'a' to 'E' distance is " << aDists[0] << ".\n";
        }
    }
}