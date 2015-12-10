g++ impl/concrete/$1_test.cpp -I. -o bin/concrete/$1_test -O2 -std=c++11 -lm && bin/concrete/$1_test --benchmark_only "${@:2}"
