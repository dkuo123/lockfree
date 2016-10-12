#include <iostream>

using namespace std;


class Base {
public:
 Base() { cout << endl << "Base con calls foo2 "; foo2(); }
 void foo() { foo2(); }
protected:
 virtual void foo2() { cout << "Base::foo2" << endl; }
};

class Dev : public Base {
public:
 Dev() { cout << endl << "Derived con calls foo2 "; foo2(); }

protected:
 void foo2() { cout << "Derived::foo2" << endl; }
};

class B 
{
public:
   virtual void f(short) {std::cout << "B::f" << std::endl;}
};

class D : public B
{
public:
   // any of this will overload, not override B::f
   virtual void f(int) {std::cout << "D::f" << std::endl;}
   //virtual void f(short) const { std::cout << "D::f  const " << std::endl;}
   //virtual void f(short) {std::cout << "D::f  short " << std::endl;} 
   // the new C++11 added void f(short) override {} for compiler to check this for you if the signature is 100% matching. 
   // void f(short) override final {} will make it not overridable in its derived class


};

main() {
 Base b; // Base con calls foo2 Base::foo2
 b.foo(); // Base::foo2

 Dev d; // Base con calls foo2 Base::foo2    Derived con calls foo2 Derived::foo2
 cout << "calling d.foo()" << endl;
 d.foo(); // Derived::foo2

 D d2;
 d2.f(1); //D::f
 d2.f((short)(2)); //shit, this still print D::f, since it's overload B:f(), not override B:f()
                   // if add virtual void f(short) const {}, it will print D::f  const 
                   //  if add virtual void f(short) {}, it will choose the best candidate in class D, which is D::f  short 
 d2.B::f(1); // now it's B::f
}

