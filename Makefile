CXXFLAGS = -Wall -ggdb3 -Wextra -std=c++11
elevator:elevator.cc Makefile
	g++ -o elevator ${CXXFLAGS} elevator.cc
	
run: elevator
	./elevator

test_output1: elevator
	./elevator < test_input1.txt > test_output1

test_output2: elevator
	./elevator < test_input2.txt > test_output2

test1:elevator test_output1
	diff test_expected test_output1

test2:elevator test_output2
	diff test_expected2 test_output2

test: test1 test2

.PHONY: run test
