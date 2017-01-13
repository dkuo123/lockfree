#include <stdio.h>

int g = 10;
auto kitten = [=]() { return g+1;};
auto cat = [x=g]() { return x+1;}; // or [g=g] to make it more confusing, but it's the same 
auto ocelto = [g]() { return g+1; }; //g++ gives warning, clang fail to compile

void global_g() {
 g = 20;
 printf("kitten = %d, cat = %d\n", kitten(), cat());
}

void local_g() {
 int g = 20;
 printf("kitten = %d, cat = %d\n", kitten(), cat());
}

template<typename T>
T sum(T v) { return v;}

template<typename T, typename... Args>
auto sum(T v1, Args... vs ) {
 return v1 + sum(vs...);
}

auto plus = [value = 1] (auto ... p) { return sum(p... , value); };
main() {
 local_g(); // kitten = 11, cat = 11 , lambda function only binds to global g, no local g
 global_g(); // kitten = 21, cat = 11, kitten use glocal g at call time, while cat will make x a local copy of g at compile time
 printf("plus(42,3.14,1) = %lf\n", plus(42,3.14,1)); // 42+3.14+1 + 1 = 47.14
}

