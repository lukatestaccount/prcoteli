#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "boost/concept_check.hpp"
#include "concepts/concrete/dynamic_array.hpp"
#include "testing/test.h"

// Implementations begin
#include "impl/kalinov/vector_backed_dynamic_array.hpp"
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
class DynamicArrayTests : public Tests {
 public:
  DynamicArrayTests(string const& class_name) : class_name_(class_name) {
    BOOST_CONCEPT_ASSERT((concepts::IsDynamicArray<T>));

    AddTest("Simple", std::bind(&DynamicArrayTests::TestSimple, this));
    AddTest("InsertAndRemoveAll",
            std::bind(&DynamicArrayTests::TestInsertAndRemoveAll, this));
    AddTest("ConstructAndRemoveAll",
            std::bind(&DynamicArrayTests::TestConstructAndRemoveAll, this));
    AddTest("CompareToVectorBackedDynamicArray1",
            std::bind(&DynamicArrayTests::TestCompareToVectorBackedDynamicArray,
                      this, /*seed=*/1));
    AddTest("CompareToVectorBackedDynamicArray2",
            std::bind(&DynamicArrayTests::TestCompareToVectorBackedDynamicArray,
                      this, /*seed=*/2));
    AddTest("CompareToVectorBackedDynamicArray3",
            std::bind(&DynamicArrayTests::TestCompareToVectorBackedDynamicArray,
                      this, /*seed=*/3));

    AddBenchmark("InsertThreeRemoveTwo",
                 std::bind(&DynamicArrayTests::BenchmarkInsertThreeRemoveTwo,
                           this, placeholders::_1, placeholders::_2));
  }

  string ClassName() override { return class_name_; }

  bool TestSimple() {
    T array;
    array.Insert(0, Data(1));
    array.Insert(0, Data(2));
    array.Insert(2, Data(3));
    array.Insert(2, Data(4));
    if (array.size() != 4) return false;
    if (array[0].value != 2) return false;
    if (array[1].value != 1) return false;
    if (array[2].value != 4) return false;
    if (array[3].value != 3) return false;
    array.Remove(1);
    if (array.size() != 3) return false;
    if (array[0].value != 2) return false;
    if (array[1].value != 4) return false;
    if (array[2].value != 3) return false;
    return true;
  }

  bool TestInsertAndRemoveAll() {
    default_random_engine random(1234);
    T array;
    InsertRandomElements(100, array, random);
    RemoveRandomElements(100, array, random);
    return array.size() == 0;
  }

  bool TestConstructAndRemoveAll() {
    default_random_engine random(1234);
    T array(100);
    RemoveRandomElements(100, array, random);
    return array.size() == 0;
  }

  bool TestCompareToVectorBackedDynamicArray(int seed) {
    default_random_engine random_ref(seed);
    default_random_engine random_test(seed);
    kalinov::VectorBackedDynamicArray<Data> array_ref;
    T array_test;
    InsertRandomElements(100, array_ref, random_ref);
    InsertRandomElements(100, array_test, random_test);
    for (int iter = 0; iter < 10; ++iter) {
      RemoveRandomElements(10, array_ref, random_ref);
      RemoveRandomElements(10, array_test, random_test);
      InsertRandomElements(5, array_ref, random_ref);
      InsertRandomElements(5, array_test, random_test);
    }
    if (array_ref.size() != array_test.size()) return false;
    for (int i = 0; i < array_ref.size(); ++i) {
      if (array_ref[i].value != array_test[i].value) return false;
    }
    return true;
  }

  pair<bool, double> BenchmarkInsertThreeRemoveTwo(int level, string* message) {
    int n = 1000;
    for (int i = 0; i < level; ++i) n = n * 2;
    ostringstream oss;
    oss << "(iters = " << n << ")";
    *message = oss.str();

    default_random_engine random(1337);
    auto t1 = chrono::high_resolution_clock::now();
    T array;
    int sum = 0;
    for (int i = 0; i < n; ++i) {
      InsertRandomElements(3, array, random);
      InsertRandomElements(2, array, random);
      sum += array[i].value;
    }
    // Use the sum, so compiler wouldn't optimize it away.
    if (sum == -1) *message = *message + "_unlikely_to_happen_";

    auto t2 = chrono::high_resolution_clock::now();
    auto runtime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    return make_pair(runtime.count() < 1.0, runtime.count());
  }

  template <typename Array>
  void InsertRandomElements(int n, Array& array, default_random_engine& engine) {
    auto index_generator = [&array, &engine]() {
      return uniform_int_distribution<int>(0, array.size())(engine);
    };
    auto value_generator = [&engine]() {
      return uniform_int_distribution<int>(0, 1000000)(engine);
    };
    for (int i = 0; i < n; ++i) {
      array.Insert(index_generator(), Data(value_generator()));
    }
  }

  template <typename Array>
  void RemoveRandomElements(int n, Array& array, default_random_engine& engine) {
    auto index_generator = [&array, &engine]() {
      return uniform_int_distribution<int>(0, array.size() - 1)(engine);
    };
    for (int i = 0; i < n; ++i) {
      array.Remove(index_generator());
    }
  }

 private:
  string class_name_;
};

#define ADD_IMPLEMENTATION(ClassName) \
  TESTER_ADD(new DynamicArrayTests<ClassName<Data>>(#ClassName))

BEGIN_TESTER(DynamicArray)
ADD_IMPLEMENTATION(kalinov::VectorBackedDynamicArray);
ADD_IMPLEMENTATION(kalinov::ListBackedKeyedDynamicArray);
END_TESTER()

}  // testing
