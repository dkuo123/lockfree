#include <array>
#include <iostream>
#include <memory>

struct TrivialDeleter {
   void operator()(int *i) {
      std::cout << "TRIVIAL VERSION: " << *i << std::endl;
      delete i;
   }
};

struct FakePointer : TrivialDeleter {
   int *ptr;
};

struct StateDeleter {
   int other;

   StateDeleter(int other_) : other(other_) { }

   void operator()(int *i) {
      std::cout << "STATE VERSION: " << *i + other << std::endl;
      delete i;
   }
};

void function(int *i) {
   std::cout << "FUNCTION VERSION: " << *i << std::endl;
   delete i;
}

typedef void (*int_func)(int *);

int main() {
   std::unique_ptr<int> p1 = std::make_unique<int>(3);
   std::unique_ptr<int, TrivialDeleter> p2 (new int(3));
   std::unique_ptr<int, StateDeleter> p3 (new int(4), StateDeleter(5));
   std::unique_ptr<int, int_func> p4 (new int(5), function);
   std::cout << sizeof(p1) << " " << sizeof(p2) << " " << sizeof(p3) << " " << sizeof(p4) << std::endl;
   std::cout << "FakePointer: " << sizeof(FakePointer) << std::endl;
   return 0;
}
