###########################################################
# Name: Aayush Talati
# Class account username: atalati6901
# Class information: CS 570
# Assignment information: Assignment #2, Experimental Shell (XSH)
# Filename: Makefile
###########################################################

EXEC = xsh
FILES = main.cpp Shell.cpp Parser.cpp Executor.cpp
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g -c
LFLAGS = -g
OBJECTS = $(FILES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CXX) $(LFLAGS) -o $(EXEC) $(OBJECTS)
	rm -f *.o

.cpp.o:
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -f *.o core a.out $(EXEC)

#######################[ EOF: Makefile ]###################
