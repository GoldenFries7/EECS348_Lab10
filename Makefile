# Makefile for EECS 348 Lab 10

lab10: Lab10.cpp
	g++ -std=c++17 -Wall -Wextra Lab10.cpp -o lab10

clean:
	rm -f lab10