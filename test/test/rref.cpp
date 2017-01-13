#include <iostream>

struct Logging {
   std::string s;
   Logging() {
      std::cerr << "Default constructor" << std::endl;
   }

   Logging(const Logging &l) : s(l.s) {
      std::cerr << "Copy constructor" << std::endl;
   }

   Logging(Logging &&l) : s(std::move(l.s)) {
      std::cerr << "Move constructor" << std::endl;
   }

   ~Logging() {
      std::cerr << "Destructor" << std::endl;
   }
};

void foo1(const Logging &l) {
}

Logging foo2() {
   return Logging{};
}

Logging foo3() {
   Logging l;
   return l;
}

Logging foo4(Logging l) {
   return l;
}

Logging foo5(bool flag) {
   Logging l1, l2;
   if (flag) return l1;
   else return l2;
}

/*
rref.cpp:44:19: error: invalid return type ‘Logging’ of constexpr function ‘constexpr Logging foo6(bool)’
 constexpr Logging foo6(bool flag) {
                   ^
rref.cpp:3:8: note: ‘Logging’ is not literal because:
 struct Logging {
        ^
rref.cpp:3:8: note:   ‘Logging’ has a non-trivial destructor

constexpr Logging foo6(bool flag) {
   Logging l1, l2;
   if (flag) return l1;
   else return l2;
}
*/

void foo7(const Logging &l) {
   // do nothing
}

Logging foo8(Logging &&l) {
   return std::move(l);
}

Logging foo9(Logging &&l) {
   return l;
}

int main() {
   {
      std::cout << "foo1" << std::endl;
      Logging l;
      foo1(l);
   }
   /*
foo1
Default constructor
Destructor
   */
   {
      std::cout << "foo2" << std::endl;
      Logging l = foo2();
   }
   /*
foo2
Default constructor
Destructor
    */
   {
      std::cout << "foo3" << std::endl;
      Logging l = foo3();
   }
   /*
foo3
Default constructor
Destructor

    */
   {
      std::cout << "foo4" << std::endl;
      Logging l1;
      Logging l2 = foo4(l1);
   }
   /*
foo4
Default constructor
Copy constructor
Move constructor
Destructor
Destructor
Destructor

    */
   {
      std::cout << "foo5" << std::endl;
      Logging l = foo5(false);
   }
   /*
foo5
Default constructor
Default constructor
Move constructor
Destructor
Destructor
Destructor

    */
   {
      std::cout << "foo7" << std::endl;
      Logging l;
      foo7(std::move(l));
   }
   /*
foo7
Default constructor
// because move() is &&, but foo7(Logging &), && and & is &, so no copy, move is involved, it's just like call foo7(l)
Destructor

    */
   {
      std::cout << "foo8" << std::endl;
      Logging l1;
      Logging l2 = foo8(std::move(l1));
   }
   /*
foo8
Default constructor
Move constructor //&& and && == &&
Destructor
Destructor

    */
   {
      std::cout << "foo9" << std::endl;
      Logging l1;
      Logging l2 = foo9(std::move(l1));
   }
   /*
foo9
Default constructor
Copy constructor
Destructor
Destructor

    */
   return 0;
}
