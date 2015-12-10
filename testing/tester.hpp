#ifndef __TESTING_TESTER_HPP__
#define __TESTING_TESTER_HPP__

#include <iomanip>
#include <memory>
#include <map>
#include <string>
#include <vector>

#include "testing/tests.hpp"

namespace testing {

using namespace std;

typedef pair<string, std::unique_ptr<Tests>> ClassNameAndTests;
ClassNameAndTests MakeClassNameAndTests(string const& class_name, Tests* tests) {
  ClassNameAndTests ret;
  ret.first = class_name;
  ret.second.reset(tests);
  return ret;
}

string _concept_name;
vector<ClassNameAndTests> _tests;
void InitTester();

#define TESTER_BEGIN(concept_name) \
  void InitTester() {             \
    _concept_name = #concept_name;
#define TESTER_ADD(class_name, tests) \
  _tests.push_back(MakeClassNameAndTests(class_name, tests))
#define TESTER_END() \
  }

void NoTestsRun() {
  if (_tests.empty()) {
    cerr << "No implementation for concept " << _concept_name << "\n";
  } else {
    cerr << "Couldn't find implementation matching your arguments.\n";
    cerr << "Available implementations:\n";
    for (auto const& entry : _tests) {
      cerr << "  " << entry.first << "\n";
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
  for (auto const& entry : _tests) {
    string const& class_name = entry.first;
    Tests* tests = entry.second.get();
    bool const should_run = ShouldRun(class_name, arguments);
    if (should_run) {
      cerr << "  Implementation: " << class_name << "\n";
      tests->RunAllTests();
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
  for (auto const& entry : _tests) {
    string const& class_name = entry.first;
    Tests* tests = entry.second.get();
    bool const should_run = ShouldRun(class_name, arguments);
    if (should_run) {
      cerr << "  Implementation: " << class_name << "\n";
      tests->RunAllBenchmarks();
      any = true;
    }
  }
  if (!any) {
    NoTestsRun();
  }
}

void Run(int argc, char* args[]) {
  bool test_only = false;
  bool benchmark_only = false;
  vector<string> arguments;
  for (int i = 1; i < argc; ++i) {
    string arg(args[i]);
    if (arg == "--test_only") {
      test_only = true;
    } else if (arg == "--benchmark_only") {
      benchmark_only = true; }
    else {
      arguments.push_back(arg);
    }
  }

  InitTester();
  if (!benchmark_only) RunTests(arguments);
  if (!test_only) RunBenchmarks(arguments);
}

}  // testing

#define TESTER_MAIN()                \
  int main(int argc, char* args[]) { \
    testing::Run(argc, args);        \
    return 0;                        \
  }

#endif  // __TESTING_TESTER_HPP__
