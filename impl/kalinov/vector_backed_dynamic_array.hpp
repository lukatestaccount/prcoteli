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
  value_type const& operator[](int index) const { return a[index]; }
  value_type& operator[](int index) { return a[index]; }
  void Insert(int index, value_type value) {
    a.insert(a.begin() + index, value);
  }
  void Remove(int index) { a.erase(a.begin() + index); }

 private:
  vector<value_type> a;
};

}  // namespace kalinov

#endif  // __IMPL_KALINOV_VECTOR_BACKED_DYNAMIC_ARRAY_HPP__
