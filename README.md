## Benchmark Lib
##### Created by Wojciech Gruszka

###Basic benchmark

```objectivec
/* lambda calculating factorial of given number */
auto factorialFunc = [](std::size_t n) {
    std::size_t res = 1;
    for (auto i = 0; i < n; i++)
        res = res *= i;
    };
    
/* Test cases form 0 to 1000000 with step 100000 */
bm::Range testCases(0, 1000000, 100000);

/* Create benchmark, run it, and show results in console */
bm::Benchmark("Factorial", factorialFunc, testCases)
    .run()
    .exportFancy(std::cout);    
```

###Benchmark Suites
```objectivec
/* lambda test function */
auto testFuncA = [](std::size_t n) {
    ...
    };
    
/* lambda test function */
auto testFuncB = [](std::size_t n) {
    ...
    };
    
/* Test cases form 0 to 1000000 with step 100000 */
bm::Range testCases(0, 1000000, 100000);

/* Create suite, run it, and show results in console */
bm::BenchmarkSuite("SampleSuite")
            .addBenchmark(bm::Benchmark("A", testFuncA, testCases))
            .addBenchmark(bm::Benchmark("B", testFuncB, testCases))
            .run()
            .exportFancy(std::cout);

```

##Library modules
###bm::Range
Represents list of test cases to be used in benchmark. Can be created with 
initializer list with those values or by constructor that automatically creates values from given range
with given interval. For example:
`
bm::Range(0, 500, 100)
`
creates representation of `{0, 100, 200, 300, 400, 500}`

That could be also achieved with `bm::Range{0, 100, 200, 300, 400, 500}`

`bm::Range` has no public methods.

###bm::Benchhmark
Represents single banchmark. 
###bm::BenchmarkSuite

##Plotting results
Library includes Python script, that allows you to plot results of benchmarks, that are axported as csv.
For now that script plot only with linear axis.

##TODO
- Ability to prepare test case before starting the timer
- Fixed perferct forwarding
- GUI for plotting
- Log and SemiLog axis