CXXFLAGS = -Wall -ggdb3 -Wextra -std=c++11
elevator:elevator.cc Makefile
	g++ -o elevator ${CXXFLAGS} elevator.cc
	
run: elevator
	./elevator

test_output: elevator
	./elevator > test_output

test:elevator test_output
	diff test_expected test_output

.PHONY: run test
