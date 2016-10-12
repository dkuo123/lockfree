class MyClass1 {
template <typename T>
/* member templates may not be virtual.  */
/* virtual -- cause compile error*/ void f(T) {}
};

template<typename T> 
class A {
  virtual void foo(){}
};

template <typename T>
class MyClass {
public:
 MyClass() = default; // needed to create MyClass<double> xd;

// copy constructor with implicit type conversion
// - does not suppress implicit copy constructor
 template <typename U>
 MyClass (const MyClass<U>& x) {}
};


void f()
{
MyClass<double> xd;
MyClass<double> xd2(xd); // calls implicitly generated copy constructor
MyClass<int> xi(xd); // calls template constructor
}


main() {
 MyClass1 m1;
 A<int> a;
f();
}


