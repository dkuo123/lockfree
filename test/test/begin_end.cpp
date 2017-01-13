#include <iostream>

namespace jimmy {
   template <typename T, size_t SZ>
   struct array {
      T data[SZ];
   };

   template <typename T, size_t SZ>
   T *begin(array<T, SZ> &arr) {
      return arr.data;
   }

   template <typename T, size_t SZ>
   T *end(array<T, SZ> &arr) {
      return arr.data + SZ;
   }
}

int main() {
   jimmy::array<int, 5> arr;
   for (int &i : arr) {
      i = 34;
   }
   std::cout << arr.data[3] << std::endl;
   return 0;
}
