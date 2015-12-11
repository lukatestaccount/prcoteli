#ifndef __IMPL_CONCRETE_ARRAY_DYNAMIC_ARRAY_HPP__
#define __IMPL_CONCRETE_ARRAY_DYNAMIC_ARRAY_HPP__

// Recommended implementation (typically the best one on the benchmark):
#include "impl/kalinov/array/vector_backed_dynamic_array.hpp"
template <class T>
class DynamicArray : public kalinov::VectorBackedDynamicArray<T> {};

#endif  // __IMPL_CONCRETE_ARRAY_DYNAMIC_ARRAY_HPP__
