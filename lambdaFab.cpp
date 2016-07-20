#include <iostream>
#include <functional>

main() {
  // lambdaFab.cpp:4:14: error: use of ‘fib’ before deduction of ‘auto’
  // auto fib = [&fib](int n) {return n < 2 ? 1 : fib(n-1) + fib(n-2);};
  std::function<int(int)> fib = [&fib](int n) {return n < 2 ? 1 : fib(n-1) + fib(n-2);};
  std::cout << "fib(5) = " << fib(5) << std::endl;
}

