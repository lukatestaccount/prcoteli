// Array is a concept with these properties:
// * The type of contained values is defined as "value_type".
// * A default constructor with no arguments that creates an empty array.
// * A constructor with the initial size as an argument that creates an array of
//   that many elements.
// * Has the size() const method that returns an int or a value that
//   can be converted to int.
// * Has the operator[](int index) const method that returns the value or the
//   reference to the value at a given index.

// MutableArray is an Array with these additional properties:
// * Has the operator[](int index) method that returns the reference to the
//   value at the given index.

// InsertableArray is an Array with these additional properties:
// * Has the Insert(int index, value_type value) method. The value is inserted
//   after the specified index.

// RemovableArray is an Array with these additional properties:
// * Has the Remove method that accepts the index of an element to remove.

#ifndef __CONCEPTS_ABSTRACT_ARRAY_HPP__
#define __CONCEPTS_ABSTRACT_ARRAY_HPP__

#include "boost/concept_check.hpp"

namespace concepts {

template <class T>
struct Array {
  typedef typename T::value_type value_type;

  BOOST_CONCEPT_USAGE(Array) {
    BOOST_CONCEPT_ASSERT((boost::Assignable<value_type>));
    BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<value_type>));

    T();
    T(15);
    int size = array.size();
    value = array[0];
  }
  T const array;
  value_type value;
};

template <class T>
struct MutableArray : Array<T> {
  typedef typename T::value_type value_type;

  BOOST_CONCEPT_USAGE(MutableArray) {
    BOOST_CONCEPT_ASSERT((boost::CopyConstructible<value_type>));
    array[0] = value;
  }
  T array;
  value_type value;
};

template <class T>
struct InsertableArray : Array<T> {
  typedef typename T::value_type value_type;

  BOOST_CONCEPT_USAGE(InsertableArray) {
    BOOST_CONCEPT_ASSERT((boost::CopyConstructible<value_type>));
    array.Insert(0, value);
  }
  T array;
  value_type value;
};

template <class T>
struct RemovableArray : Array<T> {
  typedef typename T::value_type value_type;

  BOOST_CONCEPT_USAGE(RemovableArray) {
    array.Remove(0);
  }
  T array;
};

}  // namespace concepts

#endif  // __CONCEPTS_ABSTRACT_ARRAY_HPP__
