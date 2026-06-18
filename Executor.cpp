/*
 * Name: Aayush Talati
 * Class account username: atalati6901
 * Class information: CS 570
 * Assignment information: Assignment #2, Experimental Shell (XSH)
 * Filename: Executor.cpp
 *
 * Starts programs and connects pipeline stages with UNIX pipes.
 */

#include "xsh.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Return true when the executable text contains a directory component. */
bool Executor::containsSlash(const std::string& text)
{
    return text.find('/') != std::string::npos;
}

/*
 * Resolve an executable name.  Paths containing '/' are checked directly;
 * simple names are searched for in PATH so the shell can reject invalid input
 * before forking child processes.
 */
bool Executor::findExecutable(const std::string& name, std::string& resolvedPath)
{
    struct stat info;

    if (containsSlash(name))
    {
        if (stat(name.c_str(), &info) == 0 && S_ISREG(info.st_mode) && access(name.c_str(), X_OK) == 0)
        {
            resolvedPath = name;
            return true;
        }

        return false;
    }

    const char* pathEnv = std::getenv("PATH");
    if (pathEnv == NULL)
    {
        return false;
    }

    std::string pathText(pathEnv);
    std::string::size_type start = 0;

    while (start <= pathText.size())
    {
        std::string::size_type colon = pathText.find(':', start);
        std::string directory;

        if (colon == std::string::npos)
        {
            directory = pathText.substr(start);
            start = pathText.size() + 1;
        }
        else
        {
            directory = pathText.substr(start, colon - start);
            start = colon + 1;
        }

        if (directory.empty())
        {
            directory = ".";
        }

        std::string candidate = directory + "/" + name;

        if (stat(candidate.c_str(), &info) == 0 && S_ISREG(info.st_mode) && access(candidate.c_str(), X_OK) == 0)
        {
            resolvedPath = candidate;
            return true;
        }
    }

    return false;
}

/* Build the argv array expected by execv. */
std::vector<char*> Executor::buildArgv(const Command& command, std::string& resolvedPath)
{
    std::vector<char*> argv;

    argv.push_back(const_cast<char*>(resolvedPath.c_str()));

    for (std::vector<std::string>::size_type i = 1; i < command.arguments.size(); ++i)
    {
        argv.push_back(const_cast<char*>(command.arguments[i].c_str()));
    }

    argv.push_back(NULL);
    return argv;
}

/*
 * Execute one command or a whole pipeline.  Pipes are created before forking,
 * and every process closes pipe ends it does not need so readers can receive
 * end-of-file correctly.
 */
int Executor::execute(const std::vector<Command>& commands) const
{
    if (commands.empty())
    {
        return 1;
    }

    std::vector<std::string> resolvedPaths(commands.size());

    for (std::vector<Command>::size_type i = 0; i < commands.size(); ++i)
    {
        if (!findExecutable(commands[i].executable, resolvedPaths[i]))
        {
            std::cerr << "xsh: cannot execute '" << commands[i].executable
                      << "': file not found or not executable" << std::endl;
            return 1;
        }
    }

    std::vector<int> pipeFds;
    int pipeCount = static_cast<int>(commands.size()) - 1;

    for (int i = 0; i < pipeCount; ++i)
    {
        int fds[2];
        if (pipe(fds) == -1)
        {
            std::cerr << "xsh: pipe failed: " << std::strerror(errno) << std::endl;
            return 1;
        }

        pipeFds.push_back(fds[0]);
        pipeFds.push_back(fds[1]);
    }

    std::vector<pid_t> children;

    for (std::vector<Command>::size_type i = 0; i < commands.size(); ++i)
    {
        pid_t child = fork();

        if (child == -1)
        {
            std::cerr << "xsh: fork failed: " << std::strerror(errno) << std::endl;
            return 1;
        }

        if (child == 0)
        {
            if (i > 0)
            {
                int readFd = pipeFds[(i - 1) * 2];
                if (dup2(readFd, STDIN_FILENO) == -1)
                {
                    std::cerr << "xsh: dup2 failed: " << std::strerror(errno) << std::endl;
                    _exit(1);
                }
            }

            if (i < commands.size() - 1)
            {
                int writeFd = pipeFds[i * 2 + 1];
                if (dup2(writeFd, STDOUT_FILENO) == -1)
                {
                    std::cerr << "xsh: dup2 failed: " << std::strerror(errno) << std::endl;
                    _exit(1);
                }
            }

            for (std::vector<int>::size_type j = 0; j < pipeFds.size(); ++j)
            {
                close(pipeFds[j]);
            }

            std::vector<char*> argv = buildArgv(commands[i], resolvedPaths[i]);
            execv(resolvedPaths[i].c_str(), &argv[0]);

            std::cerr << "xsh: exec failed for '" << commands[i].executable
                      << "': " << std::strerror(errno) << std::endl;
            _exit(1);
        }

        children.push_back(child);
    }

    for (std::vector<int>::size_type i = 0; i < pipeFds.size(); ++i)
    {
        close(pipeFds[i]);
    }

    int finalStatus = 0;

    for (std::vector<pid_t>::size_type i = 0; i < children.size(); ++i)
    {
        int status = 0;
        if (waitpid(children[i], &status, 0) == -1)
        {
            std::cerr << "xsh: waitpid failed: " << std::strerror(errno) << std::endl;
            finalStatus = 1;
        }
        else if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            finalStatus = 1;
        }
    }

    return finalStatus;
}
