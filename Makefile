###########################################################
# CS480 Summer 2026 Term
# Assignment 2 - Makefile

# Team Members:
#  1. Aayush Talati, class account: cssc3161
#  2. Sparsh Mody, class account: cssc3001

# Account used for testing:
#cssc3161 / atalati6901
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
