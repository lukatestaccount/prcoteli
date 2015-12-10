#ifndef __TESTING_TESTS_HPP__
#define __TESTING_TESTS_HPP__

#include <iomanip>
#include <map>
#include <string>
#include <vector>

namespace testing {

using namespace std;

class Tests {
 public:
  struct BenchmarkResult {
    int max_level;
    double max_level_runtime;
    string message;
  };

  ~Tests() = default;

  void AddTest(string test_name, std::function<bool()> test_method) {
    tests_.push_back(Test{test_name, test_method});
  }

  void AddBenchmark(string benchmark_name,
                    std::function<pair<bool, double>(int, string*)> benchmark_method) {
    benchmarks_.push_back(Benchmark{benchmark_name, benchmark_method});
  }

  void RunAllTests() {
    for (Test& test : tests_) {
      cerr << "    Running test: " << test.name << " ... ";
      bool const result = test.method();
      cerr << (result ? "PASSED" : "FAILED") << "\n";
    }
  }

  void RunAllBenchmarks() {
    for (Benchmark& benchmark : benchmarks_) {
      for (int level = 0; ; ++level) {
        cerr << "    Running benchmark: " << benchmark.name << " at level "
             << level << " ... ";
        string message;
        pair<bool, double> result = benchmark.method(level, &message);
        cerr << message << " ";
        cerr << setprecision(2) << fixed << result.second << "\n";
        if (!result.first) {
          BenchmarkResult benchmark_result;
          benchmark_result.max_level = level;
          benchmark_result.max_level_runtime = result.second;
          benchmark_result.message = message;
          benchmark_results_[benchmark.name] = benchmark_result;
          break;
        }
      }
    }
  }

 protected:
  Tests() = default;

 private:
  struct Test {
    string name;
    std::function<bool()> method;
  };
  struct Benchmark {
    string name;
    std::function<pair<bool, double>(int, string*)> method;
  };

  vector<Test> tests_;
  vector<Benchmark> benchmarks_;
  map<string, BenchmarkResult> benchmark_results_;
};

#define CHECK(condition)                            \
  if (!(condition)) {                               \
    cerr << "\n      CHECK(" << #condition << ") "; \
    return false;                                   \
  }

}  // testing

#endif  // __TESTING_TESTS_HPP__
