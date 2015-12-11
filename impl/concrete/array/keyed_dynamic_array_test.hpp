#ifndef __IMPL_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_TEST_HPP__
#define __IMPL_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_TEST_HPP__

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "boost/concept_check.hpp"
#include "concepts/concrete/array/keyed_dynamic_array.hpp"
#include "impl/concrete/array/keyed_dynamic_array.hpp"
#include "impl/concrete/array/dynamic_array_test.hpp"
#include "testing/tests.hpp"

namespace testing {

using namespace std;

template <template <class> class KeyedDynamicArrayImpl>
class KeyedDynamicArrayTests : public DynamicArrayTests<KeyedDynamicArrayImpl> {
 public:
  KeyedDynamicArrayTests() {
    typedef KeyedDynamicArrayImpl<ValueType> Impl;
    BOOST_CONCEPT_ASSERT((concepts::IsKeyedDynamicArray<Impl>));
    RegisterTests();
  }

 private:
  struct ValueType {
    int value = 0;
    ValueType() {}
    ValueType(int value) : value(value) {}
  };
  typedef typename KeyedDynamicArrayImpl<ValueType>::key_type key_type;

  void RegisterTests() {
    this->AddTest("KeyedDynamicArrayTests::TestSimple",
                  std::bind(&KeyedDynamicArrayTests::TestSimple, this));
    this->AddTest(
        "KeyedDynamicArrayTests::TestRemoveFromFirstHalfGetIndexInSecondHalf",
        std::bind(&KeyedDynamicArrayTests::
                      TestRemoveFromFirstHalfGetIndexInSecondHalf,
                  this));
    this->AddTest(
        "KeyedDynamicArrayTests::TestCompareAgainstReference1",
        std::bind(&KeyedDynamicArrayTests::TestCompareAgainstReference, this,
                  /*seed=*/1));
    this->AddTest(
        "KeyedDynamicArrayTests::TestCompareAgainstReference1",
        std::bind(&KeyedDynamicArrayTests::TestCompareAgainstReference, this,
                  /*seed=*/2));
    this->AddTest(
        "KeyedDynamicArrayTests::TestCompareAgainstReference1",
        std::bind(&KeyedDynamicArrayTests::TestCompareAgainstReference, this,
                  /*seed=*/3));

    this->AddBenchmark(
        "KeyedDynamicArrayTests::"
        "BenchmarkRemoveFromFirstHalfGetIndexInSecondHalf",
        std::bind(&KeyedDynamicArrayTests::
                      BenchmarkRemoveFromFirstHalfGetIndexInSecondHalf,
                  this, placeholders::_1, placeholders::_2));
    this->AddBenchmark(
        "KeyedDynamicArrayTests::BenchmarkInsertGetAndGetIndex",
        std::bind(&KeyedDynamicArrayTests::BenchmarkInsertGetAndGetIndex, this,
                  placeholders::_1, placeholders::_2));
  }

  bool TestSimple() {
    KeyedDynamicArrayImpl<ValueType> array(4);
    key_type key_1 = array.Insert(2, ValueType(1));
    key_type key_2 = array.InsertAfterKey(key_1, ValueType(2));
    key_type key_3 = array.InsertBeforeKey(key_2, ValueType(3));
    CHECK(array.Get(key_1).value == 1);
    CHECK(array.Get(key_2).value == 2);
    CHECK(array.Get(key_3).value == 3);
    CHECK(array.GetIndex(key_1) == 2);
    CHECK(array.GetIndex(key_2) == 4);
    CHECK(array.GetIndex(key_3) == 3);
    array.Remove(6);
    array.Remove(0);
    CHECK(array.GetIndex(key_1) == 1);
    CHECK(array.GetIndex(key_2) == 3);
    CHECK(array.GetIndex(key_3) == 2);
    array.Get(key_1) = ValueType(11);
    array.Get(key_2) = ValueType(22);
    array.RemoveKey(key_3);
    CHECK(array.size() == 4);
    CHECK(array[0].value == 0);
    CHECK(array[1].value == 11);
    CHECK(array[2].value == 22);
    CHECK(array[3].value == 0);
    return true;
  }

  bool TestRemoveFromFirstHalfGetIndexInSecondHalf() {
    KeyedDynamicArrayImpl<ValueType> array;
    int n = 100;
    key_type middle = array.Insert(0, ValueType());
    vector<key_type> first_half;
    vector<key_type> second_half;
    for (int i = 0; i < n; ++i) {
      first_half.push_back(array.InsertBeforeKey(middle, ValueType()));
      second_half.push_back(array.InsertAfterKey(middle, ValueType()));
    }
    default_random_engine rng(1337);
    for (int i = 0; i < n; ++i) {
      swap(first_half[i], first_half[uniform_int_distribution<int>(0, i)(rng)]);
    }
    for (int i = 0; i < n; ++i) {
      array.RemoveKey(first_half[i]);
      for (int j = 0; j < n; ++j) {
        CHECK(array.GetIndex(second_half[j]) == 2 * n - i - j - 1);
      }
    }
    return true;
  }


  bool TestCompareAgainstReference(int seed) {
    default_random_engine rng_ref(seed);
    default_random_engine rng_test(seed);
    KeyedDynamicArray<ValueType> array_ref;
    KeyedDynamicArrayImpl<ValueType> array_test;
    InsertGetAndGetIndex(1000, &array_ref, &rng_ref);
    InsertGetAndGetIndex(1000, &array_test, &rng_test);
    CHECK(array_ref.size() == array_test.size());
    for (int i = 0; i < array_ref.size(); ++i) {
      CHECK(array_ref[i].value == array_test[i].value);
    }
    return true;
  }

  pair<bool, double> BenchmarkRemoveFromFirstHalfGetIndexInSecondHalf(
      int level, string* message) {
    int n = 1000;
    for (int i = 0; i < level; ++i) n = n * 2;
    ostringstream oss;
    oss << "(n = " << n << ")";
    *message = oss.str();

    auto t1 = chrono::high_resolution_clock::now();

    KeyedDynamicArrayImpl<ValueType> array;
    key_type middle = array.Insert(0, ValueType());
    vector<key_type> first_half;
    vector<key_type> second_half;
    for (int i = 0; i < n; ++i) {
      first_half.push_back(array.InsertBeforeKey(middle, ValueType()));
      second_half.push_back(array.InsertAfterKey(middle, ValueType()));
    }
    default_random_engine rng(1337);
    for (int i = 0; i < n; ++i) {
      swap(first_half[i], first_half[uniform_int_distribution<int>(0, i)(rng)]);
    }
    for (int i = 0; i < n; ++i) {
      array.RemoveKey(first_half[i]);
      int j = uniform_int_distribution<int>(0, n - 1)(rng);
      if (array.GetIndex(second_half[j]) != 2 * n - i - j - 1) {
        throw "The code is wrong.";
      }
    }

    auto t2 = chrono::high_resolution_clock::now();
    auto runtime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    return make_pair(runtime.count() < 1.0, runtime.count());
  }

  pair<bool, double> BenchmarkInsertGetAndGetIndex(int level, string* message) {
    int n = 1000;
    for (int i = 0; i < level; ++i) n = n * 2;
    ostringstream oss;
    oss << "(n = " << n << ")";
    *message = oss.str();

    auto t1 = chrono::high_resolution_clock::now();

    default_random_engine rng(98765);
    KeyedDynamicArrayImpl<ValueType> array;
    InsertGetAndGetIndex(n, &array, &rng);

    auto t2 = chrono::high_resolution_clock::now();
    auto runtime = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
    return make_pair(runtime.count() < 1.0, runtime.count());
  }

  template <typename A>
  void InsertGetAndGetIndex(int n, A* array, default_random_engine* rng) {
    vector<key_type> keys;
    for (int i = 0; i < n; ++i) {
      if (i % 3 == 0) {
        keys.push_back(
            array->Insert(uniform_int_distribution<int>(0, array->size())(*rng),
                          ValueType(-1)));
      } else {
        int key_index = uniform_int_distribution<int>(0, keys.size() - 1)(*rng);
        if (i % 3 == 1) {
          keys.push_back(array->InsertBeforeKey(keys[key_index], ValueType(-1)));
        } else {
          keys.push_back(array->InsertAfterKey(keys[key_index], ValueType(-1)));
        }
      }
      {
        int key_index = uniform_int_distribution<int>(0, keys.size() - 1)(*rng);
        array->Get(keys[key_index]).value = array->GetIndex(keys[key_index]);
      }
    }
  }
};

}  // testing

#endif  // __IMPL_KEYED_CONCRETE_ARRAY_DYNAMIC_ARRAY_TEST_HPP__
