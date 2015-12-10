#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "boost/concept_check.hpp"
#include "concepts/concrete/keyed_dynamic_array.hpp"
#include "testing/test.h"

// Implementations begin
#include "impl/kalinov/list_backed_keyed_dynamic_array.hpp"
// Implementations end

namespace testing {

using namespace std;

struct Data {
  int value = 0;
  Data() {}
  Data(int value) : value(value) {}
};

template <class T>
class KeyedDynamicArrayTests : public Tests {
 public:
  KeyedDynamicArrayTests(string const& class_name) : class_name_(class_name) {
    BOOST_CONCEPT_ASSERT((concepts::IsKeyedDynamicArray<T>));
  }

  string ClassName() override { return class_name_; }

 private:
  string class_name_;
};

#define ADD_IMPLEMENTATION(ClassName) \
  TESTER_ADD(new KeyedDynamicArrayTests<ClassName<Data>>(#ClassName))

BEGIN_TESTER(KeyedDynamicArray)
ADD_IMPLEMENTATION(kalinov::ListBackedKeyedDynamicArray);
END_TESTER()

}  // testing
