// DynamicArray is an Array that is mutable, insertable and removable.

#ifndef __CONCEPTS_CONCRETE_DYNAMIC_ARRAY_HPP__
#define __CONCEPTS_CONCRETE_DYNAMIC_ARRAY_HPP__

#include "boost/concept_check.hpp"
#include "concepts/abstract/array.hpp"

// Recommended implementation.
#include "impl/kalinov/vector_backed_dynamic_array.hpp"
template <class T>
class DynamicArray : public kalinov::VectorBackedDynamicArray<T> {
};

namespace concepts {
  template <class T>
  struct DynamicArray : MutableArray<T>,
                        InsertableArray<T>,
                        RemovableArray<T> {};

}  // namespace concepts

#endif  // __CONCEPTS_CONCRETE_DYNAMIC_ARRAY_HPP__
