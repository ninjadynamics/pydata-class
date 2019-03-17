#!/usr/bin/env bash
cmd="$1 -std=c++$2"
cat pydata.cpp functest.cpp > pydata_test.cpp && $cmd -Wall pydata_test.cpp -o pydata && valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./pydata && rm pydata_test.cpp
echo "Compiled using: $cmd"
