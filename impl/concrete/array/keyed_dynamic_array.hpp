#ifndef __IMPL_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_HPP__
#define __IMPL_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_HPP__

// Recommended implementation (typically the best one on the benchmark):
#include "impl/kalinov/array/list_backed_keyed_dynamic_array.hpp"
template <class T>
class KeyedDynamicArray : public kalinov::ListBackedKeyedDynamicArray<T> {};

#endif  // __IMPL_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_HPP__
