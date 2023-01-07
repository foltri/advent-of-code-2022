//
// Created by Balint Zsiga on 2022. 12. 30..
//

#include "aoc15.h"

constexpr bool IS_PART1{ false };
constexpr bool IS_TEST{ true };
constexpr int ROW{ IS_TEST ? 10 : 2000000 };
constexpr int MAX { IS_TEST ? 20 : 4000000 };
const std::string INPUT_PATH{IS_TEST ? "../test_input/aoc15.txt" : "../input/aoc15.txt"};

class Sensor
{
private:
    int m_row{};
    int m_col{};
    int m_beaconDist{};

    int getRowDist(int row) const
    {
        return std::abs(m_row - row);
    }

    int calcBeaconDist(int beaconRow, int beaconCol) const
    {
        return std::abs(m_row - beaconRow) + std::abs(m_col - beaconCol);
    }

public:
    Sensor(int row, int col, int beaconRow, int beaconCol) :
            m_row{row},
            m_col{col},
            m_beaconDist{ calcBeaconDist(beaconRow, beaconCol)}
    {}

    int getColCoverageAtROW(int row) const { return m_beaconDist - getRowDist(row); }
    int getMinColPosAtROW(int row) const { return m_col - getColCoverageAtROW(row); }
    int getMaxColPosAtROW(int row) const { return m_col + getColCoverageAtROW(row); }
    bool isTooFarFromRow(int row) const { return getRowDist(row) > m_beaconDist; }
};

int getNextInt(std::stringstream& ss)
{
    int i{};
    while(ss.peek() != '=') ss.ignore();
    ss.ignore();
    ss >> i;
    return i;
}

std::vector<Sensor> getSensors(std::string_view path)
{
    std::vector<Sensor> sensors{};

    for (auto& line : helper::readInput(path))
    {
        std::stringstream ss{line};
        int col{ getNextInt(ss) };
        int row{ getNextInt(ss) };
        int beaconCol{ getNextInt(ss) };
        int beaconRow{ getNextInt(ss) };

        Sensor s{row, col, beaconRow, beaconCol};
        sensors.push_back(s);
    }

    return sensors;
}

bool isEverythingMerged(const std::vector<std::pair<int, int>>& sensorsMinMax) { return sensorsMinMax.size() == 1; }
bool isUnmergeable(const std::vector<std::pair<int, int>>& sensorsMinMax)
{
    static size_t prevSensorMinMaxSize {0};

    if (sensorsMinMax.size() != prevSensorMinMaxSize)
    {
        prevSensorMinMaxSize = sensorsMinMax.size();
        return false;
    }

    return true;
}

void getSmallerAndLarger(const std::pair<int, int>& a, const std::pair<int, int>& b, std::pair<int, int>& smaller, std::pair<int, int>& larger)
{
    if (a.first < b.first)    { smaller = a; larger = b; }
    else                      { smaller = b; larger = a; }
}

// returns true if merged
bool merge(std::pair<int, int>& mergeTarget, const std::pair<int, int>& b)
{
    std::pair<int, int> smaller, larger;
    getSmallerAndLarger(mergeTarget, b, smaller, larger);
    if (smaller.second+1 >= larger.first)   // +1 as they can be next to each other
    {
        mergeTarget.first = smaller.first;
        mergeTarget.second = std::max(smaller.second, larger.second);
        return true;
    }
    return false;
}

bool isColCovered(const std::vector<std::pair<int, int>>& sensorsMinMax, int col)
{
    for (auto& smm : sensorsMinMax)
    {
        if (smm.first <= col && smm.second >= col) return true;
    }

    return false;
}

void mergeAll(std::vector<std::pair<int, int>>& sensorsMinMax)
{
    std::vector<std::pair<int, int>> unmerged;
    unmerged.reserve(sensorsMinMax.size());

    while(!isUnmergeable(sensorsMinMax) && !isEverythingMerged(sensorsMinMax))  // isUnmergeable() should go first to not get skipped since it has to update a static variable every time
    {
        auto ssmToMerge { sensorsMinMax[0] };
        for (size_t i{1}; i<sensorsMinMax.size(); ++i)
        {
            bool didMerge { merge(ssmToMerge, sensorsMinMax[i]) };
            if (!didMerge)
            {
                unmerged.push_back(sensorsMinMax[i]);
            }
        }
        unmerged.push_back(ssmToMerge);
        sensorsMinMax = std::move(unmerged);
    }
}

std::vector<std::pair<int, int>> getSensorsMinMaxInRow(const std::vector<Sensor>& sensors, int row)
{
    std::vector<std::pair<int, int>> sensorsMinMax;
    sensorsMinMax.reserve(sensors.size());
    for (auto& sensor : sensors)
    {
        if (!sensor.isTooFarFromRow(row))
        {
            sensorsMinMax.emplace_back( sensor.getMinColPosAtROW(row), sensor.getMaxColPosAtROW(row) );
        }
    }
    return sensorsMinMax;
}

size_t getNumOfCoveredColsInRow(const std::vector<Sensor>& sensors, int row)
{
    auto sensorsMinMax { getSensorsMinMaxInRow(sensors, row) };
    mergeAll(sensorsMinMax);

    size_t count{0};
    for (auto& smm : sensorsMinMax)
    {
        assert(smm.second - smm.first > 0 && "Interval cannot be under 0.");
        count += smm.second - smm.first;
    }
    return count;
}


// returns -1 if there's no uncovered Col
int getUncoveredColInRow(const std::vector<Sensor>& sensors, int row, size_t max)
{
    auto sensorsMinMax { getSensorsMinMaxInRow(sensors, row) };
    mergeAll(sensorsMinMax);

    // if not all is merged we found an uncovered col
    if (sensorsMinMax.size() > 1)
    {
        assert(sensorsMinMax.size() == 2 && "This only works if unmerged has two items at max");

        std::pair<int, int> smaller, larger;
        getSmallerAndLarger(sensorsMinMax[0], sensorsMinMax[1], smaller, larger);
        return smaller.second + 1;
    }

    // check if boundaries are covered
    if (!isColCovered(sensorsMinMax, 0))
        return 0;

    if (!isColCovered(sensorsMinMax, max))
        return max;

    // everything is covered in the row
    return -1;
}


namespace aoc15
{
    void run()
    {
        auto sensors{ getSensors(INPUT_PATH) };

        if (IS_PART1)
        {
            std::cout << "Num of positions that cannot contain a beacon: " << getNumOfCoveredColsInRow(sensors, ROW);
        }
        else
        {
            for (size_t row{0}; row <= MAX; ++row)
            {
                if (row % 100000 == 0)
                    std::cout << "starting row " << row << " from " << MAX << '\n';

                auto uncoveredCol { getUncoveredColInRow(sensors, row, MAX) };
                if (uncoveredCol != -1)
                {
                    std::cout << "row: " << row << " uncoveredCol: " << uncoveredCol << '\n';
                    unsigned long tuningFreq { uncoveredCol * 4000000UL + row };
                    std::cout << "the tuning frequency is: " << tuningFreq << '\n';
                    break;
                }
            }
        }
    }
}