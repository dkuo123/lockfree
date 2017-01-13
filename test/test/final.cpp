#include <iostream>
using namespace std;

class IAbstract
{
public:
  virtual void DoSomething() = 0;
  void Blah( void ) { DoSomething(); }
};

class D1 : public IAbstract
{
public:
  void DoSomething() override { cout << "D1" << endl; }

};

class D2 : public IAbstract {
public:
   void DoSomething() final { cout << "D2" << endl ; }
};

class D3 : public D1 {
public:
   void DoSomething() final {cout << "D3" << endl ; }
};



main() {
  IAbstract *p = new D3();
  p->Blah();
}
