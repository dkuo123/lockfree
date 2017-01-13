#include <iostream>
#include <complex>
#include <memory>
using namespace std;

class base {
public:
    virtual void f( int );
    virtual void f( double );
    virtual void g( int i = 10 );
    virtual ~base() { cout << "~base()" << endl;}
};

void base::f( int ) {
    cout << "base::f(int)" << endl;
}

void base::f( double ) {
    cout << "base::f(double)" << endl;
}

void base::g( int i ) {
    cout << i << endl;
}

class derived: public base {
public:
    void f( complex<double> );
    void g( int i = 20 ) /* override */; 
    virtual ~derived() { cout << "~derived()" << endl;}
};

void derived::f( complex<double> ) {
    cout << "derived::f(complex)" << endl;
}

void derived::g( int i ) {
    cout << "derived::g() " << i << endl;
}

int main() {
    base    b;
    derived d;
    base*   pb = new derived;

    b.f(1.0); // base::f(double)
    d.f(1.0); // derived::f(complex), because it hides the base::f(), to make them exposed, add using base::f;
    pb->f(1.0); // base::f(double), because overload resolution is done on the static type(here base), not the dynamic type( derive), you have a base pointer, so it uses base interface

    b.g(); // 10
    d.g(); // derived::g() 20
    pb->g(); // interesting again, it uses base interface, the default value is from static type(here is base). so it prints  derived::g() 10

    delete pb; // should use unique_ptr to auto handle new, delete
    {
	unique_ptr<base> pb3(new derived());
    }
    {
	cout << "unique_ptr<base> pb2 = make_unique<derived>();  this is only C++14 " << endl;
	unique_ptr<base> pb2 = make_unique<derived>();  //this is only C++14     g++ -std=c++14 virtualFunction.cpp 
	pb2->g(); // derived::g() 10
	auto pb = make_unique<derived>();
	pb->g(); // derived::g() 20   because it's like unique_ptr<derived> pb2 = make_unique<derived>(); its default value statically binds to derived interface at compile time
	// this auto force it back to base
	auto pb3 = unique_ptr<base>{ make_unique<derived>() };
	pb3->g();// derived::g() 10
    }
    
}
