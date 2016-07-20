#include <iostream>

using namespace std;

class B {
public:
 virtual void foo(int i ) {}
 virtual void foo(char c) {}
};

class D: public B {
public:
   void foo(int i ) override {cout << "D:foo(int)" << endl; }
};

int main() {
D d;
d.foo(1);
}

