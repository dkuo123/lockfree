#include <iostream>

template<typename T>
bool pair_comparer(T a, T b) {
  // In real-world code, we wouldn't compare floating point values like
  // this. It would make sense to specialize this function for floating
  // point types to use approximate comparison.
  //std::cout << __PRETTY_FUNCTION__ << "\n";  // with this cout, compiler can't optimize it anymore
  return a == b;
}

template<typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
  // std::cout << __PRETTY_FUNCTION__ << "\n";
  return a == b && pair_comparer(args...);
}

main() {
 bool b=  pair_comparer(1.5, 1.5, 2, 2, 6, 6); // compiler will optimized to true if -O3
 double d1 = 1.2, d2=1.2;
 int i1 = 1, i2 = 1, i3=3,i4=4;
 b = pair_comparer(d1,d2,i1,i2,i3,i4,10,10); // compiler can optimize it to false 
 int i5;
 std::cout << "input an int: ";
 std::cin >> i5;
 b = pair_comparer(d1,d2,i1,i2,i3,i5,10,10); // compiler will optimize it as i5 == 3
 std::cout << b << std::endl;
 return b;
}
