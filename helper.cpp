//
// Created by Balint Zsiga on 2022. 12. 03..
//

#include "helper.h"

namespace helper
{
    std::vector<std::string> readInput(std::string_view file_name) {
        // ifstream is used for reading files
        // We'll read from a file called Sample.txt
        std::ifstream inf{file_name};

        std::vector<std::string> input{};

        // If we couldn't open the input file stream for reading
        if (!inf) {
            // Print an error and exit
            std::cerr << "Uh oh, teh file could not be opened for reading!\n";
            assert(false && "Uh oh, teh file could not be opened for reading!\n");
        }

        // While there's still stuff left to read
        while (inf) {
            // read stuff from the file into a string and print it
            std::string strInput;
            std::getline(inf, strInput);
            input.push_back(strInput);
        }

        // remove last empty element
        input.pop_back();

        return input;
        // When inf goes out of scope, the ifstream
        // destructor will close the file
    }
}