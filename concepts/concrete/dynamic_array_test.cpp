// DynamicArray is an Array that is mutable, insertable and removable.

#include <iostream>
#include <functional>
#include <string>
#include <vector>

#include "concepts/concrete/dynamic_array.hpp"
#include "testing/test.h"

// Implementations begin
#include "impl/kalinov/vector_backed_dynamic_array.hpp"
// Implementations end

namespace testing {

struct Data {
  string text;
  Data() {}
  Data(string const& text) : text(text) {}
};

template <class T>
class DynamicArrayTests : public Tests {
 public:
  DynamicArrayTests(string const& class_name) : class_name_(class_name) {
    Add("Simple", [this]() {
      T array;
      array.Insert(0, Data("first"));
      array.Insert(0, Data("second"));
      array.Insert(2, Data("third"));
      array.Insert(2, Data("fourth"));
      if (array.size() != 4) return false;
      if (array[0].text != "second") return false;
      if (array[1].text != "first") return false;
      if (array[2].text != "fourth") return false;
      if (array[3].text != "third") return false;
      array.Remove(1);
      if (array.size() != 3) return false;
      if (array[0].text != "second") return false;
      if (array[1].text != "fourth") return false;
      if (array[2].text != "third") return false;
      return true;
    });
    Add("Random", [this]() {
      srand(1337);
      T array;
      for (int iter = 0; iter < 100; ++iter) {
        int const index = rand() % (array.size() + 1);
        array.Insert(index, Data(GetRandomText()));
      }
      return true;
    });
  }

  string ClassName() override { return class_name_; }

  string GetRandomText() {
    int len = rand() % 5 + 5;
    string text;
    for (int i = 0; i < len; ++i) text += rand() % 26 + 'a';
    return text;
  }

 private:
  string class_name_;
};

#define ADD_IMPLEMENTATION(ClassName) \
  ADD_TESTS(new DynamicArrayTests<ClassName<Data>>(#ClassName))

BEGIN_TESTER(DynamicArray)
ADD_IMPLEMENTATION(kalinov::VectorBackedDynamicArray);
END_TESTER()

}  // testing
