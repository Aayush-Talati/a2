CS480 Summer 2026 Term
Assignment 2 - README File 

Team Members:
1. Aayush Talati, class account: cssc3161
2. Sparsh Mody, class account: cssc3001

Account used for testing:
cssc3161 / atalati6901

File Manifest:
README: contains the project description, build instructions, the instructions to operate/run the code, and anything else the assignment requires.
main.cpp: this file creates the Shell object and starts the execution for the whole program.
xsh.h: this file contains all of the class declarations, function prototypes used for the project, and all necessary structures.
Shell.cpp: this file implements all of the shell prompt and the loop forf the reading/parsing. 
Parser.cpp: This file gets the user input and turns it into executable commands for the program via parsing.
Executor.cpp: This file executes the commands, searches the Path for executables, makes all of the processes and sets the foundation for the subtasls/subprocesses
Makefile: This file has all of the build instructions for compilation and creation of executable.

Compiling Instructions:
Use "make" to compile the project.
The Makefile creates an executable named "xsh".
Use "make clean" to remove generated object files and the executable.

Run the shell using: ./xsh
The shell will display a prompt using the USER environment variable: username%
Supported command formats: executable
executable argument
executable | executable
executable | executable | executable
exit

Design Decisions:
1. The project was divided into separate source files to improve readability and maintainability.
2. The Parser class is responsible only for validating and parsing user input.
3. The Executor class handles all process creation and execution logic.
4. PATH searching is performed manually before forking so invalid commands can be rejected immediately.
5. Multiple pipes are supported by creating all required pipes before child processes are created.
6. The prompt is generated using the USER environment variable and falls back to "cssc0000%" if unavailable.
7. The system() function is not used anywhere in the project as required by the assignment.

Extra Features Not Required:
1. Provides descriptive error messages for invalid command formats.

Known Bugs:
No known bugs at this time based on our testing 

Lessons Learned:
1. We learned how UNIX shells create and manage processes.
2. We learned how fork(), execv(), waitpid(), and pipe() work together.
3. We learned how inter-process communication is implemented using pipes.
4. We learned how PATH searching works when locating executables.
5. We learned how to organize larger C++ projects using multiple source files and classes.

Work Attribution: 
Both team members were able to collaborate really easily. We split work up and work together via github. +
Aayush Talati:
Shell.cpp
Parser.cpp
Executor.cpp
Makefile
README
Did Final testing in Edoras


Sparsh Mody:
xsh.h
main.cpp
Went thrugh each file and set up what needs to be included
Did final checks and testing before turning in to make sure all criteria met 




