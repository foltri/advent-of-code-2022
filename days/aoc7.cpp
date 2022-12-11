//
// Created by Balint Zsiga on 2022. 12. 07..
//

#include "aoc7.h"

namespace aoc7
{
    struct Fiile
    {
        int size{};
    };

    class Dir
    {
    private:
        std::vector<Fiile> m_files{};
        std::vector<Dir> m_dirs{};
        int m_fsize{};
        char m_name{};  // for debugging
        Dir* m_prevDir{ nullptr};

        static std::vector<int> s_sizeList;

    public:
        explicit Dir(const char name) : m_name{name}
        {

        }

        Dir& addDirectory(const Dir& dir)
        {
            m_dirs.push_back(dir);
            m_dirs.back().m_prevDir = this;
            return m_dirs.back();
        }

        void addFile(const Fiile& file)
        {
            m_files.push_back(file);
            m_fsize += file.size;
        }

        // this recursively calculates sizes from containing folder sizes
        int getSize()
        {
            int chiledDirFsize{ 0 };
            for (auto dir: m_dirs)
            { chiledDirFsize += dir.getSize(); }

            int size{m_fsize + chiledDirFsize};
            s_sizeList.push_back(size);

            return size;
        }

        Dir* getPrevDir() { return m_prevDir; }

        static std::vector<int> getSizeList() { return s_sizeList; }
        static void clearSizeList() { s_sizeList.clear(); }

    };

    // init s_sizeList
    std::vector<int> Dir::s_sizeList{};

    Dir ParseDirs(std::vector<std::string>& data)
    {
        Dir root{'/'};
        Dir* dirPtr{ &root };

        for (size_t i{1}; i<data.size(); ++i)
        {
            // get first 3 chars to match for needed task
            std::string_view str{ data[i].substr(0, 3) };

            // save dirs when entering them with cd
            if (str == "$ c")
            {
                char name {data[i].back()};
                if (name != '.') dirPtr = &dirPtr->addDirectory(Dir{name});
                else dirPtr = dirPtr->getPrevDir();
            }

            // list and dirs
            else if (str == "$ l" || str == "dir") continue;

            // files
            else
            {
                int size { stoi(data[i].substr(0, data[i].find(' '))) };
                dirPtr->addFile(Fiile{ size });
            }
        }
        return root;
    }

    void run()
    {
        auto data { helper::readInput("../input/aoc7.txt") };
        Dir root { ParseDirs(data) };

        int currentSize{ root.getSize() };
        int minSpaceToFeeUp{ currentSize - 40000000 };
        std::cout << "The whole size of root is: " << currentSize << '\n';

        int sum{0};
        int closestSize{99999999};
        for (auto size : aoc7::Dir::getSizeList())
        {
            if (size <= 100000) sum += size;
            if (size >= minSpaceToFeeUp && size < closestSize) closestSize = size;
        }

        std::cout << "The sum of the directories with total size at most 100000 is: " << sum << '\n';
        std::cout << "Size of the directory to clean up: " << closestSize << '\n';
    }
}