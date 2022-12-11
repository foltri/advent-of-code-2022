//
// Created by Balint Zsiga on 2022. 12. 09..
//

#ifndef ADVENT_OF_CODE_2022_AOC8_H
#define ADVENT_OF_CODE_2022_AOC8_H

#include <map>

#include "../helper.h"

enum class Side
{
    left,
    right,
    top,
    bottom
};

class TreeGrid;

class Tree
{
private:
    int m_height{};
    std::map<Side, bool> m_visible{};
    std::map<Side, int> m_viewingDistance{};
public:
    explicit Tree(int height);

    int getHeight() const;
    std::map<Side, int>& getViewingDistance();

    void setVisibility(Side side, bool visible);
    void setViewingDistance(Side side, int dist);

    bool isVisible() const;

    void calcLeftViewDistance(TreeGrid* grid, size_t row, size_t col);
    void calcRightViewDistance(TreeGrid* grid, size_t row, size_t col);
    void calcTopViewDistance(TreeGrid* grid, size_t row, size_t col);
    void calcBottomViewDistance(TreeGrid* grid, size_t row, size_t col);
};

class TreeGrid
{

private:
    std::vector<std::vector<Tree>> m_grid{ };
    void calcVisibilityOfTree(Tree &tree, int &maxHeight, Side side);

public:
    TreeGrid(size_t numCols, size_t numRows);
    void addTree(size_t row, const Tree &tree);
    Tree& getTree(size_t row, size_t col);
    size_t numRows() const;
    size_t numCols() const;
    void calcLeftVisibility();
    void calcRightVisibility();
    void calcTopVisibility();
    void calcBottomVisibility();
    int getNumOfVisible() const;
    void calcViewingDistances();
    int getHighestScenicScore();
};

namespace aoc8
{
    void run();
}

#endif //ADVENT_OF_CODE_2022_AOC8_H
