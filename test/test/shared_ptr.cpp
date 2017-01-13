#include <iostream>
#include <boost/shared_ptr.hpp>
//include <memory>
using namespace std;
class B {
public:
  virtual void foo() { cout << "BASE" << endl; }
};
class D : public B {
public:
  virtual void foo() { cout << "Derive" << endl; }
};

int main() {
 
  boost::shared_ptr<D> dptr(new D());
  const boost::shared_ptr<D>& dptr2{dptr};
  boost::shared_ptr<B> bptr(dptr2);
  bptr->foo();
}
