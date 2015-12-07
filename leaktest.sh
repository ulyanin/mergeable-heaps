#!/usr/bin/bash

g++ --std=c++11 -lgtest -Wall *.cpp && valgrind --leak-check=full -v --show-leak-kinds=all ./a.out
