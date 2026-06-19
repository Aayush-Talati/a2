// CS480 Summer 2026 Term
// Assignment 2 - Shell.cpp

// Team Members:
// 1. Aayush Talati, class account: cssc3161
// 2. Sparsh Mody, class account: cssc3001

// Account used for testing:
// cssc3161 / atalati6901
// Implements the interactive read/parse/execute loop for XSH.
 

#include "xsh.h"

#include <cstdlib>
#include <iostream>
#include <string>

/*
 * Build the prompt from the USER environment variable.  If USER is not set,
 * use the class-account placeholder from the assignment description.
 */
std::string Shell::prompt()
{
    const char* user = std::getenv("USER");

    if (user == NULL || std::string(user).empty())
    {
        return "cssc0000% ";
    }

    return std::string(user) + "% ";
}

/*
 * Run the shell until the user enters exit or the input stream closes.
 */
int Shell::run()
{
    std::string line;

    while (true)
    {
        std::cout << prompt();
        std::cout.flush();

        if (!std::getline(std::cin, line))
        {
            std::cout << std::endl;
            break;
        }

        ParseResult result = parser.parseLine(line);

        if (result.shouldExit)
        {
            break;
        }

        if (!result.valid)
        {
            std::cerr << "xsh: " << result.errorMessage << std::endl;
            continue;
        }

        executor.execute(result.commands);
    }

    return 0;
}
