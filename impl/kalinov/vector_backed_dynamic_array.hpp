#ifndef __IMPL_KALINOV_VECTOR_BACKED_DYNAMIC_ARRAY_HPP__
#define __IMPL_KALINOV_VECTOR_BACKED_DYNAMIC_ARRAY_HPP__

#include <vector>

namespace kalinov {

using namespace std;

template<typename T>
class VectorBackedDynamicArray {
 public:
  typedef T value_type;

  VectorBackedDynamicArray() {}
  VectorBackedDynamicArray(int n) : a(n) {}
  int size() const { return a.size(); }
  const T& operator[](int index) const { return a[index]; }
  T& operator[](int index) { return a[index]; }
  void Insert(int index, T value) { a.insert(a.begin() + index, value); }
  void Remove(int index) { a.erase(a.begin() + index); }

 private:
  vector<T> a;
};

}  // namespace kalinov

#endif  // __IMPL_KALINOV_VECTOR_BACKED_DYNAMIC_ARRAY_HPP__
