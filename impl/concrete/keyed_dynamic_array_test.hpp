#ifndef __IMPL_CONCRETE_KEYED_DYNAMIC_ARRAY_TEST_HPP__
#define __IMPL_CONCRETE_KEYED_DYNAMIC_ARRAY_TEST_HPP__

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "boost/concept_check.hpp"
#include "concepts/concrete/keyed_dynamic_array.hpp"
#include "impl/concrete/keyed_dynamic_array.hpp"
#include "impl/concrete/dynamic_array_test.hpp"
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
  }

  bool TestSimple() {
    KeyedDynamicArrayImpl<ValueType> array(4);
    key_type key_1 = array.Insert(2, ValueType(1));
    key_type key_2 = array.InsertAfterKey(key_1, ValueType(2));
    key_type key_3 = array.InsertBeforeKey(key_2, ValueType(3));
    CHECK(array.Get(key_1).value == 1);
    CHECK(array.Get(key_2).value == 2);
    CHECK(array.Get(key_3).value == 3);
    CHECK(array.size() == 7);
    array.Remove(6);
    array.Remove(0);
    CHECK(array.size() == 5);
    CHECK(array[0].value == 0);
    CHECK(array[1].value == 1);
    CHECK(array[2].value == 3);
    CHECK(array[3].value == 2);
    CHECK(array[4].value == 0);
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

  bool TestInsertAndRemoveAllByKey() {
    return true;
  }
};

}  // testing

#endif  // __IMPL_KEYED_CONCRETE_DYNAMIC_ARRAY_TEST_HPP__
