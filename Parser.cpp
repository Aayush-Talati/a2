// CS480 Summer 2026 Term
// Assignment 2 - README File 

// Team Members:
// 1. Aayush Talati, class account: cssc3161
// 2. Sparsh Mody, class account: cssc3001

// Account used for testing:
// cssc3161 / atalati6901
// Checks the command line against the limited syntax required for XSH.
 

#include "xsh.h"

#include <cctype>
#include <sstream>

/* Remove spaces and tabs from both ends of a string. */
std::string Parser::trim(const std::string& text)
{
    std::string::size_type first = 0;
    std::string::size_type last = text.size();

    while (first < last && std::isspace(static_cast<unsigned char>(text[first])))
    {
        ++first;
    }

    while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1])))
    {
        --last;
    }

    return text.substr(first, last - first);
}

/* Split a command segment into whitespace-separated tokens. */
std::vector<std::string> Parser::splitWhitespace(const std::string& text)
{
    std::vector<std::string> tokens;
    std::istringstream input(text);
    std::string token;

    while (input >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

/*
 * Parse the line.  Legal commands are:
 *   executable
 *   executable argument
 *   executable [argument] | executable [argument] | ...
 *   exit
 * More than one argument for any command is rejected.
 */
ParseResult Parser::parseLine(const std::string& line) const
{
    ParseResult result;
    result.valid = false;
    result.shouldExit = false;

    std::string cleaned = trim(line);

    if (cleaned.empty())
    {
        result.errorMessage = "empty command";
        return result;
    }

    if (cleaned == "exit")
    {
        result.valid = true;
        result.shouldExit = true;
        return result;
    }

    if (cleaned.find("exit") != std::string::npos)
    {
        std::vector<std::string> exitCheck = splitWhitespace(cleaned);
        if (!exitCheck.empty() && exitCheck[0] == "exit")
        {
            result.errorMessage = "exit does not accept arguments or pipes";
            return result;
        }
    }

    std::string::size_type start = 0;

    while (start <= cleaned.size())
    {
        std::string::size_type pipe = cleaned.find('|', start);
        std::string segment;

        if (pipe == std::string::npos)
        {
            segment = cleaned.substr(start);
            start = cleaned.size() + 1;
        }
        else
        {
            segment = cleaned.substr(start, pipe - start);
            start = pipe + 1;
        }

        segment = trim(segment);

        if (segment.empty())
        {
            result.errorMessage = "missing command near pipe";
            result.commands.clear();
            return result;
        }

        std::vector<std::string> tokens = splitWhitespace(segment);

        if (tokens.size() > 2)
        {
            result.errorMessage = "each command may have at most one argument";
            result.commands.clear();
            return result;
        }

        if (tokens[0] == "exit")
        {
            result.errorMessage = "exit cannot be used in a pipeline";
            result.commands.clear();
            return result;
        }

        Command command;
        command.executable = tokens[0];
        command.arguments.push_back(tokens[0]);

        if (tokens.size() == 2)
        {
            command.arguments.push_back(tokens[1]);
        }

        result.commands.push_back(command);
    }

    result.valid = !result.commands.empty();
    return result;
}
