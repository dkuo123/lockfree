#include <cstddef>
#include <iostream>

template <class T>
struct dumb_ptr {
   T *ptr;
   dumb_ptr() = default;
   dumb_ptr(dumb_ptr<T> &d) = default;
   dumb_ptr(T *p) : ptr(p) { }
   T &operator *() { return *ptr; }
   T *operator ->() { return ptr; }
   void dereference_print() {
      print(*(*this));
   }

private:
   template <typename T2>
   static void print(const T2 &t) {
      std::cout << "[" << t << std::endl;
   }

   static void print(int i) {
      std::cout << "[" << i << "]" << std::endl;
   }
};

/*
// full specialization
template <>
struct dumb_ptr<int> {
   int *ptr;
   dumb_ptr() = default;
   dumb_ptr(dumb_ptr<int> &d) = default;
   dumb_ptr(int *p) : ptr(p) { }
   int &operator *() { return *ptr; }
   int *operator ->() { return ptr; }
   int val() { return *ptr; }
};
*/

// partial specialization
template <typename T>
struct dumb_ptr<T[]> {
   T *ptr;
   dumb_ptr() = default;
   dumb_ptr(T *p) : ptr(p) { }
   T &operator[](size_t index) {
      return ptr[index];
   }
   void dereference_print(size_t index) {
     print(*((*this)[index]));
   }
};

int main() {
   dumb_ptr<int> ip (new int(3));
   dumb_ptr<char> cp (new char(3));
   ip.dereference_print();
   cp.dereference_print();
   dumb_ptr<char[]> ap(new char[5]);
   ap.derefence_print(2);
   return 0;
}
