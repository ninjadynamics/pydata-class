#!/usr/bin/env bash
cat pydata.cpp functest.cpp > pydata_test.cpp && g++ pydata_test.cpp -o pydata && valgrind --leak-check=full --track-origins=yes ./pydata
rm pydata_test.cpp
