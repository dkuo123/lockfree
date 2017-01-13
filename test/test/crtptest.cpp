#include <iostream>
//Curiously recurring template pattern, https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
// Some use cases for this pattern are static polymorphism
// This technique achieves a similar effect to the use of virtual functions, without the costs (and some flexibility) of dynamic polymorphism
template <typename Derived, typename T>
class Base {
    Derived &GetDerived() {
        return static_cast<Derived &>(*this);
    } // NOTE, can't return an object of Derived, will cause recursive

public:
    T parentVal;

    void printAllData() {
        GetDerived().print();
    }
};

template <typename T>
class Derived : public Base<Derived<T>, T> {
public:
    T childVal;

    void print() {
        //std::cout << "Parent val: " << parentVal << std::endl;   //Doesn't work
        std::cout << "Parent val: " << Base<Derived<T>, T>::parentVal << std::endl;
        std::cout << "Child val: " << childVal << std::endl;
    }
};

template<typename T>
class virtualClass {
public:
  virtual void foo() {std::cout << "virtualClass" << std::endl;}
};

class V2: public virtualClass<int> {
public:
   void foo() override {std::cout << "V2" << std::endl;}
};

template <typename T>
struct counter
{
    static int objects_created;
    static int objects_alive;

    counter()
    {
        ++objects_created;
        ++objects_alive;
    }
    
    counter(const counter&)
    {
        ++objects_created;
        ++objects_alive;
    }
protected:
    ~counter() // objects should never be removed through pointers of this type
    {
        --objects_alive;
    }
};
template <typename T> int counter<T>::objects_created( 0 );
template <typename T> int counter<T>::objects_alive( 0 );

class X : public counter<X>
{
    // ...
};

class Y : counter<Y>
{
    // ...
};

// Polymorphic copy construction
// Base class has a pure virtual function for cloning
class Shape {
public:
    virtual ~Shape() {};
    virtual Shape *clone() const = 0;
  virtual void foo() const { std::cout << "I am Shape" << std::endl;}
};
// This CRTP class implements clone() for Derived
template <typename Derived>
class Shape_CRTP : public Shape {
public:
    virtual Shape *clone() const {
        return new Derived(static_cast<Derived const&>(*this));
    }
};

// Nice macro which ensures correct CRTP usage
#define Derive_Shape_CRTP(Type) class Type: public Shape_CRTP<Type>

// Every derived class inherits from Shape_CRTP instead of Shape
Derive_Shape_CRTP(Square) {
 public:
  virtual void foo() const { std::cout << "I am Square" << std::endl;}
};
Derive_Shape_CRTP(Circle) {
 public:
  virtual void foo() const { std::cout << "I am Circle" << std::endl;}
};

#include <typeinfo>

int main() {
    Derived<int> var;
    var.childVal = 5;
    var.parentVal = 10;
    var.printAllData();
    X x;
    {
      X x2(x);
    }
    std::cout << "X created = " << X::objects_created << ", alive = " << x.objects_alive << std::endl;
    Shape *p = new Circle();
    p->clone()->foo();
    std::cout <<"p is circle ptr: " << typeid(p).name() << ", " << typeid(*(p->clone())).name() << std::endl;
    p = new Square();
    p->clone()->foo();
    std::cout << "p is square ptr: " << typeid(p).name() << ", " << typeid(*(p->clone())).name() << std::endl;
    p = nullptr;
    std::cout << "p is nullptr: " << typeid(p).name() << ", "  << typeid(p->clone()).name() << std::endl; // seg fault as *(p->clone()) is deref nullptr 

    virtualClass<int> *p2 = new V2(); // surprisely it compiles OK until the next line
    // std::cout << p2->foo() << std::endl; // this can't compile as it need dynamically binding and template can't have virtual function called

    auto i = atoi("0"), j = atoi("foo");
    std::cout << "i = " << i << ", j = " << j << std::endl;
    return 0;
    
}
