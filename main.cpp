#include <iostream>
#include "Benchmark.h"

int main() {

    /* lambda calculating factorial of given number */
    auto factorialFunc = [](std::size_t n) {
        std::size_t res = 1;
        for (auto i = 0; i < n; i++)
            res = res *= i;
    };

    /* Test cases form 0 to 1000000 with step 1000 */
    bm::Range testCases(0, 1000000, 10000);
    /* Create benchmark, run it, and show results in console */
    bm::Benchmark("Factorial", factorialFunc, testCases).run().exportFancy(std::cout);

}