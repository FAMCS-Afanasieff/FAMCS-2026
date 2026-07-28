set -e
g++ -O2 -std=c++17 -o overlap overlap.cpp
printf "1 1\n1 1\n" | ./overlap mode1 2 1 2
printf "1 1\n1 1\n" | ./overlap mode1 2 1 3
printf "1 1\n1 1\n" | ./overlap mode1 2 1 1
printf "1 2 2\n0 0 0\n0 0 0\n" | ./overlap mode2 3 2 1 2
printf "1 2 2\n0 0 0\n0 0 0\n" | ./overlap mode2 3 2 2 1
./overlap mode1 7 7 2 0 1 1 2 1 1 < examples/pdf_pattern.txt
./overlap count1 2 2 1 1
./overlap count2 2 2 1 2
