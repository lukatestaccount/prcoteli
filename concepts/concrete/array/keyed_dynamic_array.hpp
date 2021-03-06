// KeyedDynamicArray is a DynamicArray with these additional properties:
// * The class defines the "key_type" which can be used to refer to inserted
//   elements even if they move in the array.
// * The Insert(int index, value_type value) method now returns key_type, the
//   key of the inserted element.
// * Has InsertBeforeKey(key_type key, value_type value) that inserts a value
//   before the element specified by the key, returning the key of the inserted
//   element.
// * Has InsertAfterKey(key_type key, value_type value) that inserts a value
//   after the element specified by the key, returning the key of the inserted
//   element.
// * Has RemoveKey(key_type key) method that removes the element specified by
//   the key.
// * Has Get(key_type key) const method which returns the value for a given key.
// * Has Get(key_type key) method which returns a reference to the value for a
//   given key.
// * Has GetIndex(key_type key) const method which returns the index of the
//   element specified by the key.

#ifndef __CONCEPTS_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_HPP__
#define __CONCEPTS_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_HPP__

#include "boost/concept_check.hpp"
#include "concepts/concrete/array/dynamic_array.hpp"

namespace concepts {

template <class T>
struct IsKeyedDynamicArray : IsDynamicArray<T> {
  typedef typename T::key_type key_type;
  typedef typename T::value_type value_type;

  BOOST_CONCEPT_USAGE(IsKeyedDynamicArray) {
    BOOST_CONCEPT_ASSERT((boost::CopyConstructible<value_type>));
    key = array.Insert(0, value);
    key = array.InsertAfterKey(key, value);
    key = array.InsertBeforeKey(key, value);
    array.RemoveKey(key);
    value = const_array.Get(key);
    array.Get(key) = value;
    index = const_array.GetIndex(key);
  }
  T array;
  T const const_array;
  key_type key;
  value_type value;
  int index;
};

}  // namespace concepts

#endif  // __CONCEPTS_CONCRETE_ARRAY_KEYED_DYNAMIC_ARRAY_HPP__
