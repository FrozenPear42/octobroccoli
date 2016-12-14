#include <initializer_list>
#include <string>
#include <functional>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <map>
#include <list>
#include <fstream>
#include <vector>

namespace bm {

    class Range {
    public:
        Range(std::size_t pBegin, std::size_t pEnd, std::size_t pInterval) {
            if (pBegin > pEnd)
                std::swap(pBegin, pEnd);
            for (auto i = pBegin; i <= pEnd; i += pInterval)
                mElements.emplace_back(i);
        }

        Range(std::initializer_list<std::size_t>&& pList) {
            for (auto&& item : pList)
                mElements.emplace_back(item);
        }

        std::vector<std::size_t>::iterator begin() {
            return mElements.begin();
        }

        std::vector<std::size_t>::iterator end() {
            return mElements.end();
        }
    private:
        std::vector<std::size_t> mElements;
    };

    class Benchmark {
    public:
        template<typename TestFuncT, typename ListT>
        Benchmark(std::string pName, TestFuncT pFunc, ListT pCases) : mName(pName),
                                                                      mTestFunc(std::forward<TestFuncT>(pFunc)) {
            for (auto&& item : pCases)
                mResults.insert(std::make_pair(item, -1.0));
        };

        template<typename PrepareT, typename CallbackT>
        Benchmark& run(PrepareT pPrepareFunc, CallbackT pCallbackFunc) {
            pPrepareFunc();
            using namespace std::chrono;
            time_point<system_clock> start, end;
            duration<double> elapsed;
            int i = 0;
            for (auto&& item : mResults) {
                i += 100;
                start = system_clock::now();
                mTestFunc(item.first);
                end = system_clock::now();
                elapsed = end - start;
                item.second = elapsed.count();
                pCallbackFunc(item, (int) (i / mResults.size()));
            }
            return *this;
        }

        template<typename CallbackT>
        Benchmark& run(CallbackT pCallbackFunc) {
            using namespace std::chrono;
            time_point<system_clock> start, end;
            int i = 0;
            for (auto&& item : mResults) {
                i += 100;
                start = system_clock::now();

                mTestFunc(item.first);

                end = system_clock::now();
                duration<double> elapsed = end - start;
                item.second = elapsed.count();
                pCallbackFunc(item, (int) (i / mResults.size()));
            }
            return *this;
        }

        Benchmark& run() {
            return run([](std::pair<const int, double>, int) {});
        }

        Benchmark& exportFancy(std::ostream& pOut) {
            pOut << "Benchmark: " << mName << "\n";
            for (auto&& result : mResults)
                pOut << result.first << "\t\t\t" << std::setprecision(10) << result.second << "\n";
            return *this;
        }

        Benchmark& exportCSV(std::ostream& pOut) {
            pOut << "N," << mName << "\n";
            for (auto&& result : mResults)
                pOut << result.first << "," << std::setprecision(10) << result.second << "\n";
            return *this;
        }

    private:
        std::string mName;
        std::function<void(std::size_t)> mTestFunc;
        std::map<int, double> mResults;
    };


    class BenchmarkSuite {
    public:

        BenchmarkSuite(std::string pName) : mName(pName), mReady(false) {};

        template<typename Tt>
        BenchmarkSuite(std::string pName, std::initializer_list<Tt>&& pBenchmarks) : BenchmarkSuite(pName) {
            for (auto&& item : pBenchmarks)
                mBenchmarks.emplace_back(std::forward<Tt>(item));
        }

        template<typename Tt>
        BenchmarkSuite& addBenchmark(Tt&& pBenchmark) {
            mBenchmarks.emplace_back(std::forward<Tt>(pBenchmark));
            mReady = false;
            return *this;
        }

        template<typename Tt>
        BenchmarkSuite& run(Tt pCallback) {
            int i = 0;
            for (auto&& item : mBenchmarks) {
                i += 1;
                item.run([&](std::pair<const int, double> pPair, int pPercent) {
                    //FIXME: change percentage calculation
                    pCallback(pPair, (int) ((pPercent * i) / mBenchmarks.size()));
                });
            }
            mReady = true;
            return *this;
        }

        BenchmarkSuite& run() {
            for (auto&& item : mBenchmarks)
                item.run();
            mReady = true;
            return *this;
        }


        BenchmarkSuite& exportFancy(std::ostream& pOut) {
            if (!mReady)
                throw std::logic_error("Trying to print unready benchmark results");
            pOut << "Benchmark Suite: " << mName << "\n\n";
            for (auto&& bench : mBenchmarks) {
                bench.exportFancy(pOut);
                pOut << "\n";
            }
            return *this;
        }

        BenchmarkSuite& exportCSV(std::ostream& pOut) {
            if (!mReady)
                throw std::logic_error("Trying to print unready benchmark results");
            pOut << mName << "\n\n";
            for (auto&& bench : mBenchmarks) {
                bench.exportCSV(pOut);
                pOut << "\n";
            }
            return *this;
        }

        BenchmarkSuite& exportCSVFile(std::string pName = "") {
            if (pName.empty())
                pName = mName + ".csv";
            std::ofstream file(pName.c_str());
            if (file) {
                exportCSV(file);
                file.flush();
                file.close();
            }
            return *this;
        }

    private:
        std::string mName;
        std::list<Benchmark> mBenchmarks;
        bool mReady;
    };
}