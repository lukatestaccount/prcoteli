#ifndef __TESTING_TEST_H__
#define __TESTING_TEST_H__

#include <iomanip>
#include <memory>
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

  virtual string ClassName() = 0;

  void AddTest(string test_name, std::function<bool()> test_method) {
    tests_.push_back(Test{test_name, test_method});
  }

  void AddBenchmark(string benchmark_name,
                    std::function<pair<bool, double>(int, string*)> benchmark_method) {
    benchmarks_.push_back(Benchmark{benchmark_name, benchmark_method});
  }

  void RunAllTests() {
    cerr << "  Implementation: " << ClassName() << "\n";
    for (Test& test : tests_) {
      cerr << "    Running test: " << test.name << " ... ";
      bool const result = test.method();
      cerr << (result ? "PASSED" : "FAILED") << "\n";
    }
  }

  void RunAllBenchmarks() {
    cerr << "  Implementation: " << ClassName() << "\n";
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

string _concept_name;
vector<std::unique_ptr<Tests>> _tests;
void InitTester();

#define BEGIN_TESTER(concept_name) \
  void InitTester() {             \
    _concept_name = #concept_name;
#define TESTER_ADD(tests) _tests.emplace_back(tests)
#define END_TESTER() \
  }

void NoTestsRun() {
  if (_tests.empty()) {
    cerr << "No implementation for concept " << _concept_name << "\n";
  } else {
    cerr << "Couldn't find implementation matching your arguments.\n";
    cerr << "Available implementations:\n";
    for (auto const& t : _tests) {
      cerr << "  " << t->ClassName() << "\n";
    }
  }
}

bool ShouldRun(string const& class_name, vector<string> const& arguments) {
  if (arguments.empty()) return true;
  for (string const& arg : arguments) {
    if (class_name.find(arg) != string::npos) return true;
  }
  return false;
}

void RunTests(vector<string> const& arguments) {
  cerr << "Running tests for concept: " << _concept_name << "\n";
  bool any = false;
  for (auto const& t : _tests) {
    bool const should_run = ShouldRun(t->ClassName(), arguments);
    if (should_run) {
      t->RunAllTests();
      any = true;
    }
  }
  if (!any) {
    NoTestsRun();
  }
}

void RunBenchmarks(vector<string> const& arguments) {
  cerr << "Running benchmarks for concept: " << _concept_name << "\n";
  bool any = false;
  for (auto const& t : _tests) {
    bool const should_run = ShouldRun(t->ClassName(), arguments);
    if (should_run) {
      t->RunAllBenchmarks();
      any = true;
    }
  }
  if (!any) {
    NoTestsRun();
  }
}

}  // testing

int main(int argc, char* args[]) {
  bool test_only = false;
  bool benchmark_only = false;
  std::vector<std::string> arguments;
  for (int i = 1; i < argc; ++i) {
    std::string arg(args[i]);
    if (arg == "--test_only") {
      test_only = true;
    } else if (arg == "--benchmark_only") {
      benchmark_only = true; }
    else {
      arguments.push_back(arg);
    }
  }

  testing::InitTester();
  if (!benchmark_only) testing::RunTests(arguments);
  if (!test_only) testing::RunBenchmarks(arguments);
  return 0;
}

#endif  // __TESTING_TEST_H__
