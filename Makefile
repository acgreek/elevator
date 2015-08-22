CXXFLAGS = -Wall -ggdb3 -Wextra -std=c++11
elevator:elevator.cc Makefile
	g++ -o elevator ${CXXFLAGS} elevator.cc
	
