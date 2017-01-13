#pragma rdfaa
struct S {
  S() : v3{12} {};
  int v = 5;
  int v2 = 10;
  int v3;
  double d = 1.5;
};

main() {
  
 //atomic<int> i(5);
 // ++i;
  S s;
  return s.v+ s.v2 + s.v3 + s.d;
}
