// KeyedDynamicArray is a DynamicArray with these additional properties:
// * The class defines the "key_type" which can be used to refer to inserted
//   elements even if they move in the array.
// * The Insert(int index, value_type value) method now returns key_type, the
//   key of the inserted element.
// * Has Get(key_type key) const method which returns the value for a given key.
// * Has Get(key_type key) method which returns a reference to the value for a
//   given key.
// * Has InsertBeforeKey(key_type key, value_type value) that inserts a value
//   before the element specified using the key, returning the key of the
//   inserted element.
// * Has InsertAfterKey(key_type key, value_type value) that inserts a value
//   after the element specified using the key, returning the key of the
//   inserted element.
// * Has RemoveKey(key_type key) method that removes the element specified using
//   the key.

#ifndef __CONCEPTS_CONCRETE_KEYED_DYNAMIC_ARRAY_HPP__
#define __CONCEPTS_CONCRETE_KEYED_DYNAMIC_ARRAY_HPP__

#include "boost/concept_check.hpp"
#include "concepts/concrete/dynamic_array.hpp"

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
    value = const_array.Get(key);
    array.Get(key) = value;
    array.RemoveKey(key);
  }
  T array;
  T const const_array;
  key_type key;
  value_type value;
};

}  // namespace concepts

#endif  // __CONCEPTS_CONCRETE_KEYED_DYNAMIC_ARRAY_HPP__
