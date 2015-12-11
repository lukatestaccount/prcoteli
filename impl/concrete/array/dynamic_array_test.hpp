#ifndef __IMPL_CONCRETE_ARRAY_DYNAMIC_ARRAY_TEST_HPP__
#define __IMPL_CONCRETE_ARRAY_DYNAMIC_ARRAY_TEST_HPP__

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "boost/concept_check.hpp"
#include "concepts/concrete/array/dynamic_array.hpp"
#include "impl/concrete/array/dynamic_array.hpp"
#include "testing/tests.hpp"

namespace testing {

using namespace std;

template <template <class> class DynamicArrayImpl>
class DynamicArrayTests : public Tests {
 public:
  DynamicArrayTests() {
    typedef DynamicArrayImpl<ValueType> Impl;
    BOOST_CONCEPT_ASSERT((concepts::IsDynamicArray<Impl>));
    RegisterTests();
  }

 private:
  struct ValueType {
    int value = 0;
    ValueType() {}
    ValueType(int value) : value(value) {}
  };

  void RegisterTests() {
    this->AddTest("DynamicArrayTests::TestSimple",
                  std::bind(&DynamicArrayTests::TestSimple, this));
    this->AddTest("DynamicArrayTests::TestInsertAndRemoveAll",
                  std::bind(&DynamicArrayTests::TestInsertAndRemoveAll, this));
    this->AddTest(
        "DynamicArrayTests::TestConstructAndRemoveAll",
        std::bind(&DynamicArrayTests::TestConstructAndRemoveAll, this));
    this->AddTest("DynamicArrayTests::TestBinarySearchInsertionSort",
                  std::bind(&DynamicArrayTests::TestBinarySearchInsertionSort, this));
    this->AddTest(
        "DynamicArrayTests::TestCompareAgainstRecommended1",
        std::bind(&DynamicArrayTests::TestCompareAgainstRecommended, this,
                  /*seed=*/1));
    this->AddTest(
        "DynamicArrayTests::TestCompareAgainstRecommended2",
        std::bind(&DynamicArrayTests::TestCompareAgainstRecommended, this,
                  /*seed=*/2));
    this->AddTest(
        "DynamicArrayTests::TestCompareAgainstRecommended3",
        std::bind(&DynamicArrayTests::TestCompareAgainstRecommended, this,
                  /*seed=*/3));

    this->AddBenchmark(
        "DynamicArrayTests::BenchmarkBinarySearchInsertionSort",
        std::bind(&DynamicArrayTests::BenchmarkBinarySearchInsertionSort, this,
                  placeholders::_1, placeholders::_2));
    this->AddBenchmark(
        "DynamicArrayTests::BenchmarkInsertThreeRemoveTwo",
        std::bind(&DynamicArrayTests::BenchmarkInsertThreeRemoveTwo, this,
                  placeholders::_1, placeholders::_2));
    this->AddBenchmark(
        "DynamicArrayTests::BenchmarkConstructAndAccess",
        std::bind(&DynamicArrayTests::BenchmarkConstructAndAccess, this,
                  placeholders::_1, placeholders::_2));
  }

  bool TestSimple() {
    DynamicArrayImpl<ValueType> array;
    array.Insert(0, ValueType(1));
    array.Insert(0, ValueType(2));
    array.Insert(2, ValueType(3));
    array.Insert(2, ValueType(4));
    CHECK(array.size() == 4);
    CHECK(array[0].value == 2);
    CHECK(array[1].value == 1);
    CHECK(array[2].value == 4);
    CHECK(array[3].value == 3);
    array.Remove(1);
    CHECK(array.size() == 3);
    CHECK(array[0].value == 2);
    CHECK(array[1].value == 4);
    CHECK(array[2].value == 3);
    return true;
  }

  bool TestInsertAndRemoveAll() {
    default_random_engine rng(1234);
    DynamicArrayImpl<ValueType> array;
    InsertRandomElements(100, &array, &rng);
    RemoveRandomElements(100, &array, &rng);
    CHECK(array.size() == 0);
    return true;
  }

  bool TestConstructAndRemoveAll() {
    default_random_engine rng(1234);
    DynamicArrayImpl<ValueType> array(100);
    RemoveRandomElements(100, &array, &rng);
    CHECK(array.size() == 0);
    return true;
  }

  bool TestCompareAgainstRecommended(int seed) {
    default_random_engine rng_ref(seed);
    default_random_engine rng_test(seed);
    DynamicArray<ValueType> array_ref;
    DynamicArrayImpl<ValueType> array_test;
    InsertRandomElements(100, &array_ref, &rng_ref);
    InsertRandomElements(100, &array_test, &rng_test);
    for (int iter = 0; iter < 10; ++iter) {
      RemoveRandomElements(10, &array_ref, &rng_ref);
      RemoveRandomElements(10, &array_test, &rng_test);
      InsertRandomElements(5, &array_ref, &rng_ref);
      InsertRandomElements(5, &array_test, &rng_test);
    }
    CHECK(array_ref.size() == array_test.size());
    for (int i = 0; i < array_ref.size(); ++i) {
      CHECK(array_ref[i].value == array_test[i].value);
    }
    return true;
  }

  bool TestBinarySearchInsertionSort() {
    default_random_engine rng(12345);
    vector<int> numbers;
    for (int i = 0; i < 1000; ++i) {
      numbers.push_back(uniform_int_distribution<int>(0, 1000)(rng));
    }
    DynamicArrayImpl<int> array;
    BinarySearchInsertionSort(numbers, &array);
    CHECK(array.size() == 1000);
    sort(numbers.begin(), numbers.end());
    for (int i = 0; i < 1000; ++i) {
      CHECK(array[i] == numbers[i]);
    }
    return true;
  }

  pair<bool, double> BenchmarkBinarySearchInsertionSort(int level, string* message) {
    int n = 1000;
    for (int i = 0; i < level; ++i) n = n * 2;
    ostringstream oss;
    oss << "(n = " << n << ")";
    *message = oss.str();

    default_random_engine rng(1337);
    vector<int> numbers;
    for (int i = 0; i < n; ++i) {
      numbers.push_back(uniform_int_distribution<int>(0, n)(rng));
    }
    auto t1 = chrono::high_resolution_clock::now();

    DynamicArrayImpl<int> array;
    BinarySearchInsertionSort(numbers, &array);

    auto t2 = chrono::high_resolution_clock::now();
    auto runtime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    return make_pair(runtime.count() < 1.0, runtime.count());
  }

  pair<bool, double> BenchmarkInsertThreeRemoveTwo(int level, string* message) {
    int n = 1000;
    for (int i = 0; i < level; ++i) n = n * 2;
    ostringstream oss;
    oss << "(iters = " << n << ")";
    *message = oss.str();

    default_random_engine rng(1337);
    auto t1 = chrono::high_resolution_clock::now();

    DynamicArrayImpl<ValueType> array;
    for (int i = 0; i < n; ++i) {
      InsertRandomElements(3, &array, &rng);
      InsertRandomElements(2, &array, &rng);
      array[i].value = i;
    }

    auto t2 = chrono::high_resolution_clock::now();
    auto runtime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    return make_pair(runtime.count() < 1.0, runtime.count());
  }

  pair<bool, double> BenchmarkConstructAndAccess(int level, string* message) {
    int n = 1000;
    for (int i = 0; i < level; ++i) n = n * 2;
    int iters = 50 * n;
    ostringstream oss;
    oss << "(n = " << n << ", iters = " << iters << ")";
    *message = oss.str();

    default_random_engine rng(1337);
    auto t1 = chrono::high_resolution_clock::now();

    DynamicArrayImpl<ValueType> array(n);
    long long sum = 0;
    for (int i = 0; i < iters; ++i) {
      int index = uniform_int_distribution<int>(0, n - 1)(rng);
      sum += array[index].value;
    }
    if (sum != 0) {
      throw "The code is wrong.";
    }
    auto t2 = chrono::high_resolution_clock::now();
    auto runtime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    return make_pair(runtime.count() < 1.0, runtime.count());
  }

  void BinarySearchInsertionSort(vector<int> const& numbers,
                                 DynamicArrayImpl<int>* array) {
    for (int x : numbers) {
      int lo = 0, hi = array->size();
      while (lo != hi) {
        int mid = (lo + hi) / 2;
        if ((*array)[mid] >= x) {
          hi = mid;
        } else {
          lo = mid + 1;
        }
      }
      array->Insert(lo, x);
    }
  }

  template <typename A>
  void InsertRandomElements(int n, A* array, default_random_engine* rng) {
    auto index_generator = [array, rng]() {
      return uniform_int_distribution<int>(0, array->size())(*rng);
    };
    auto value_generator = [rng]() {
      return uniform_int_distribution<int>(0, 1000000)(*rng);
    };
    for (int i = 0; i < n; ++i) {
      array->Insert(index_generator(), ValueType(value_generator()));
    }
  }

  template <typename A>
  void RemoveRandomElements(int n, A* array, default_random_engine* rng) {
    auto index_generator = [array, rng]() {
      return uniform_int_distribution<int>(0, array->size() - 1)(*rng);
    };
    for (int i = 0; i < n; ++i) {
      array->Remove(index_generator());
    }
  }
};

}  // testing

#endif  // __IMPL_CONCRETE_ARRAY_DYNAMIC_ARRAY_TEST_HPP__
