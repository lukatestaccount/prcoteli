#ifndef __TESTING_TEST_H__
#define __TESTING_TEST_H__

#include <memory>
#include <string>
#include <vector>

namespace testing {

using namespace std;

class Tests {
 public:
  virtual string ClassName() = 0;

  void Add(string test_name, std::function<bool()> test_method) {
    tests_.push_back(Test{test_name, test_method});
  }

  void RunAll() {
    cerr << "  Implementation: " << ClassName() << "\n";
    for (Test& test : tests_) {
      cerr << "    Running test: " << test.name << " ... ";
      test.result = test.method();
      cerr << (test.result ? "PASSED" : "FAILED") << "\n";
    }
  }

 private:
  struct Test {
    string name;
    std::function<bool()> method;
    bool result;
  };
  vector<Test> tests_;
};

string _concept_name;
vector<std::unique_ptr<Tests>> _tests;
void InitTester();

#define BEGIN_TESTER(concept_name) \
  void InitTester() {             \
    _concept_name = #concept_name;
#define ADD_TESTS(tests) _tests.emplace_back(tests)
#define END_TESTER() \
  }

void RunAll() {
  InitTester();
  cerr << "Concept: " << _concept_name << "\n";
  for (auto const& t : _tests) {
    t->RunAll();
  }
}

}  // testing

int main() {
  testing::RunAll();
  return 0;
}

#endif  // __TESTING_TEST_H__
