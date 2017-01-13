#include <iostream>

using namespace std;
struct BType {
  BType(): a(88) {}
  int a;
};

struct DType : BType {
  DType(): a("DType") {}
  //string s;
  string a;
};


struct Base {
 Base():i(100) { p = new BType(); }
 int i;
 BType *p;
 virtual void foo() {  cout << "B::i = " << i << ", BType.a = " << p->a << endl;}
};

struct Derive : Base {
 Derive() : i("ABC") { p = new DType();}
 void foo() override { 
	//cout << "D::i = " << i << ", DType.s = " << p->s << endl;
	cout << "D::i = " << i << ", DType.a = " << p->a << endl; 
	Base::foo();  
}
 string i;
 //BType *p; 
 DType *p;
};

main() {
  Base *p = new Derive();
  p->foo();
  p = new Base();
  p->foo();
}
