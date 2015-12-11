#include "impl/concrete/array/dynamic_array_test.hpp"
#include "testing/tester.hpp"

// A list of DynamicArray implementations:
#include "impl/kalinov/array/vector_backed_dynamic_array.hpp"
#include "impl/kalinov/array/list_backed_keyed_dynamic_array.hpp"

namespace testing {

#define TESTER_ADD_IMPLEMENTATION(ClassName) \
  TESTER_ADD(#ClassName, new DynamicArrayTests<ClassName>())

TESTER_BEGIN(DynamicArray)
TESTER_ADD_IMPLEMENTATION(kalinov::VectorBackedDynamicArray);
TESTER_ADD_IMPLEMENTATION(kalinov::ListBackedKeyedDynamicArray);
TESTER_END()

}  // testing

TESTER_MAIN()
