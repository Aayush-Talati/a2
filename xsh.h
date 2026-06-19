/*
 * Name: Aayush Talati
 * Class account username: atalati6901
 * Class information: CS 570
 * Assignment information: Assignment #2, Experimental Shell (XSH)
 * Filename: xsh.h
 *
 * Shared declarations for the XSH program.
 */

#ifndef XSH_H
#define XSH_H

#include <string>
#include <vector>

/* A single executable and its optional single argument. */
struct Command
{
    std::string executable;
    std::vector<std::string> arguments;
};

/* Result returned by the command-line parser. */
struct ParseResult
{
    bool valid;
    bool shouldExit;
    std::string errorMessage;
    std::vector<Command> commands;
};

class Parser
{
public:
    ParseResult parseLine(const std::string& line) const;

private:
    static std::string trim(const std::string& text);
    static std::vector<std::string> splitWhitespace(const std::string& text);
};

class Executor
{
public:
    int execute(const std::vector<Command>& commands) const;

private:
    static bool findExecutable(const std::string& name, std::string& resolvedPath);
    static bool containsSlash(const std::string& text);
    static std::vector<char*> buildArgv(const Command& command, std::string& resolvedPath);
};

class Shell
{
public:
    int run();

private:
    Parser parser;
    Executor executor;

    static std::string prompt();
};

#endif
