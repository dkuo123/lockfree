#include <iostream>
#include <typeinfo>

using namespace std;

class BASE {
virtual void hello() {}  // it must have virtual to make dynamic_cast work
};

class DE: public BASE {
int i;
};

class B {
public:
  virtual void foo(BASE& c);
};

class D : public B {
public:
  //override can't follow by implementation 
  //void foo(BASE& c) override { cout << "D::foo() " << endl; }
  void foo(BASE& c)  override;

};

void B::foo(BASE& c) { cout << "B::foo() " << endl; }
void D::foo(BASE& c) { cout << "D::foo() " << endl; 
   try {
       DE &d = dynamic_cast<DE& >(c);
   }
   catch(bad_cast& e) {
       cout << "c isn't type DE" << endl;
  }
}

main() {
 BASE c;
 D d;
 d.foo(c);
 cout << "again passing DE" << endl;
 DE dc;
 d.foo(dc);
}
