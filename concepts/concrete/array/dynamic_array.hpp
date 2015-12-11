// DynamicArray is an Array that is mutable, insertable and removable.

#ifndef __CONCEPTS_CONCRETE_ARRAY_DYNAMIC_ARRAY_HPP__
#define __CONCEPTS_CONCRETE_ARRAY_DYNAMIC_ARRAY_HPP__

#include "boost/concept_check.hpp"
#include "concepts/abstract/array/array.hpp"

namespace concepts {

template <class T>
struct IsDynamicArray : IsMutableArray<T>,
                        IsInsertableArray<T>,
                        IsRemovableArray<T> {};

}  // namespace concepts

#endif  // __CONCEPTS_CONCRETE_ARRAY_DYNAMIC_ARRAY_HPP__
