#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <iostream>

struct A {
   int a;
   std::string b;

   A(const A &other) : a(other.a), b(other.b) {
      std::cerr << "A(const A&)" << std::endl;
   }

   A(A &&other) : a(other.a), b(std::move(other.b)) {
      std::cerr << "A(A&&)" << std::endl;
   }

   A(int i) : a(i) {
      std::cerr << "A(int)" << std::endl;
   }

   A(int i, std::string s) : a(i), b(std::move(s)) {
      std::cerr << "A(int, std::string)" << std::endl;
   }

   bool operator <(const A& other) const {
      return std::tie(a, b) < std::tie(other.a, other.b);
   }

   A &operator =(const A &other) = default;
   A &operator =(A &&other) = default;

   ~A() {
      std::cerr << "~A()" << std::endl;
   }
};

struct B {
   std::string c;

   B(const B &other) : c(other.c) {
      std::cerr << "B(const B&)" << std::endl;
   }

   B(B &&other) : c(std::move(other.c)) {
      std::cerr << "B(B&&)" << std::endl;
   }

   B() {
      std::cerr << "B()" << std::endl;
   }

   B(std::string s) : c(std::move(s)) {
      std::cerr << "B(std::string)" << std::endl;
   }

   bool operator<(const B &other) const {
      return c < other.c;
   }

   B &operator =(const B &other) = default;
   B &operator =(B &&other) = default;

#if 0
   B(const std::string &s) : c(s) { }
   B(std::string &&s) : c(std::move(s)) { }
#endif

   ~B() {
      std::cerr << "~B()" << std::endl;
   }
};

auto getline()
{
return std::make_tuple("foo",true);
}

int main() {
   int i = 3;
   std::string s("Hi!");
   {
      std::cerr << "p1" << std::endl;
      std::pair<A, B> p1(i, s); //A(int), B(std::string),~B(),~A()
   }
   {
      std::cerr << "p2" << std::endl;
      std::pair<A, B> p2(std::piecewise_construct,
                         std::forward_as_tuple(i, s),
                         std::forward_as_tuple());
      //"A(int, std::string), B(),~B(),~A()
   }
   {
      std::cerr << "p3" << std::endl;
      std::pair<A, B> p3{A{i, s}, B{}};
      //A(int, std::string),,B(), A(A&&), B(B&&),~B(),~A(),~B(),~A()
   }
   {
      std::cerr << "m1" << std::endl;
      std::map<A, B> m1;
      m1.emplace(std::piecewise_construct,
                 std::forward_as_tuple(i, s),
                 std::forward_as_tuple());
      //"A(int, std::string), B(),~B(),~A()
      // im map, when its node is deleted, first the value, then the key.
   }
   {
      std::cerr << "m2" << std::endl;
      std::map<A, B> m2;
      m2.emplace(A{i, s}, B{});
      // B(),A(int, std::string), A(A&&), B(B&&),~A(), ~B(),~B(),~A()
   }
   {
      std::cerr << "m3" << std::endl;
      std::map<A, B> m3;
      m3[A{i, s}]; // = B{};
      //A(int, std::string),A(A&&), B(),~A(),~B(), ~A()
   }

	{
	auto a = getline();
	string s;
	bool ok;
	std::tie(s, ok)= getline();
	std::tie(s,std::ignore)= getline();
	}
   return 0;
}
