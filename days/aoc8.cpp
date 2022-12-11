//
// Created by Balint Zsiga on 2022. 12. 09..
//

#include "aoc8.h"


Tree::Tree(const int height) : m_height{height}
{

}

int Tree::getHeight() const { return m_height; }
std::map<Side, int>& Tree::getViewingDistance() { return m_viewingDistance; }


void Tree::setVisibility(Side side, bool visible) { m_visible[side] = visible; }
void Tree::setViewingDistance(Side side, int dist) { m_viewingDistance[side] = dist; }

bool Tree::isVisible() const
{
    return std::any_of(m_visible.begin(), m_visible.end(), [](auto v){return v.second;});
}

void Tree::calcRightViewDistance(TreeGrid* grid, size_t row, size_t col)
{
    int viewingDistance{0};
    for (size_t i{col+1}; i<grid->numCols(); ++i)
    {
        auto tree { grid->getTree(row, i) };
        ++viewingDistance;
        if (m_height <= tree.getHeight()) break;
    }
    setViewingDistance(Side::right, viewingDistance);
}

void Tree::calcLeftViewDistance(TreeGrid* grid, size_t row, size_t col)
{
    int viewingDistance{0};
    if (col != 0)
    {
        for (size_t i{ col }; i != 0; --i)
        {
            auto tree{ grid->getTree(row, i-1) };
            ++viewingDistance;
            if (m_height <= tree.getHeight()) break;
        }
    }
    setViewingDistance(Side::left, viewingDistance);
}

void Tree::calcTopViewDistance(TreeGrid* grid, size_t row, size_t col)
{
    int viewingDistance{0};
    for (size_t i{row+1}; i<grid->numRows(); ++i)
    {
        auto tree { grid->getTree(i, col) };
        ++viewingDistance;
        if (m_height <= tree.getHeight()) break;
    }
    setViewingDistance(Side::top, viewingDistance);
}

void Tree::calcBottomViewDistance(TreeGrid* grid, size_t row, size_t col)
{
    int viewingDistance{0};
    if (col != 0)
    {
        for (size_t i{ row }; i != 0; --i)
        {
            auto tree{ grid->getTree(i-1, col) };
            ++viewingDistance;
            if (m_height <= tree.getHeight()) break;
        }
    }
    setViewingDistance(Side::bottom, viewingDistance);
}


void TreeGrid::calcVisibilityOfTree(Tree& tree, int& maxHeight, Side side)
{
    int currentHeight{ tree.getHeight() };
    bool visible{ maxHeight < currentHeight };

    if (visible) maxHeight = currentHeight;

    tree.setVisibility(side, visible);
}

TreeGrid::TreeGrid(const size_t numCols, const size_t numRows)
{
    m_grid.reserve(numRows);
    for(size_t row{0}; row < numRows; ++row)
    {
        std::vector<Tree> trees{};
        trees.reserve(numCols);

        m_grid.push_back(trees);
    }
}

void TreeGrid::addTree(const size_t row, const Tree& tree)
{
    m_grid.at(row).push_back(tree);
}

Tree& TreeGrid::getTree(const size_t row, const size_t col) { return m_grid[row][col]; }
size_t TreeGrid::numRows() const { return m_grid.size(); }
size_t TreeGrid::numCols() const { return m_grid[0].size(); }

void TreeGrid::calcLeftVisibility()
{
    for (auto& trees : m_grid)
    {
        int maxHeight{-1};
        for (auto& tree : trees)
        {
            calcVisibilityOfTree(tree, maxHeight, Side::left);
        }
    }
}

void TreeGrid::calcRightVisibility()
{
    for (auto& trees : m_grid)
    {
        int maxHeight{-1};
        for (auto tree{trees.rbegin()}; tree < trees.rend(); ++tree)
        {
            calcVisibilityOfTree(*tree, maxHeight, Side::right);
        }
    }
}

void TreeGrid::calcTopVisibility()
{
    for (size_t col{0}; col < m_grid[0].size(); ++col)
    {
        int maxHeight{-1};
        for (size_t row{0}; row < m_grid.size(); ++row)
        {
            auto& tree { m_grid[row][col] };
            calcVisibilityOfTree(tree, maxHeight, Side::top);
        }
    }
}

void TreeGrid::calcBottomVisibility()
{
    for (size_t col{0}; col < m_grid[0].size(); ++col)
    {
        int maxHeight{-1};
        for (size_t row1{0}; row1 < m_grid.size(); ++row1)
        {
            auto row{ m_grid.size()-row1-1 };
            auto& tree { m_grid[row][col] };

            calcVisibilityOfTree(tree, maxHeight, Side::bottom);
        }
    }
}

int TreeGrid::getNumOfVisible() const
{
    int numVisible{0};
    for (auto& row : m_grid)
    {
        for (auto& tree : row)
        {
            if (tree.isVisible()) ++numVisible;
        }
    }
    return numVisible;
}

void TreeGrid::calcViewingDistances()
{
    for (size_t row{0}; row < numRows(); ++row)
    {
        for (size_t col{0}; col < numCols(); ++col)
        {
            getTree(row, col).calcLeftViewDistance(this, row, col);
            getTree(row, col).calcRightViewDistance(this, row, col);
            getTree(row, col).calcTopViewDistance(this, row, col);
            getTree(row, col).calcBottomViewDistance(this, row, col);
        }
    }
}

int TreeGrid::getHighestScenicScore()
{
    int highest{0};
    for (size_t row{0}; row < numRows(); ++row)
    {
        for (size_t col{0}; col < numCols(); ++col)
        {
            int score{1};
            for (auto& dist : getTree(row, col).getViewingDistance())
            {
                score *= dist.second;
            }

            if (highest < score) highest = score;
        }
    }
    return highest;
}


namespace aoc8
{
    void run()
    {
        auto data {helper::readInput("../input/aoc8.txt")};
        TreeGrid grid{ data[0].length(), data.size() };

        for(size_t row{0}; row<data.size(); ++row)
        {
            for (size_t col{0}; col<data[row].length(); ++col)
            {
                grid.addTree(row, Tree{ data[row][col] - 48 });
            }
        }

        // First part
//        grid.calcLeftVisibility();
//        grid.calcRightVisibility();
//        grid.calcTopVisibility();
//        grid.calcBottomVisibility();
//        std::cout << "The number of visible trees in the grid: " << grid.getNumOfVisible() << '\n';


        // second part
        grid.calcViewingDistances();
        std::cout << "The highest scenic score is " << grid.getHighestScenicScore() << '\n';
    }
}
