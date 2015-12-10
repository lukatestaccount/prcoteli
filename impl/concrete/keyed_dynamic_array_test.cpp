#include "impl/concrete/keyed_dynamic_array_test.hpp"
#include "testing/tester.hpp"

// A list of KeyedDynamicArray implementations:
#include "impl/kalinov/list_backed_keyed_dynamic_array.hpp"

namespace testing {

#define TESTER_ADD_IMPLEMENTATION(ClassName) \
  TESTER_ADD(#ClassName, new KeyedDynamicArrayTests<ClassName>())

TESTER_BEGIN(KeyedDynamicArray)
TESTER_ADD_IMPLEMENTATION(kalinov::ListBackedKeyedDynamicArray);
TESTER_END()

}  // testing

TESTER_MAIN()
