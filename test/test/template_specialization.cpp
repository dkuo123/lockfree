#include <iostream>

using namespace std;

template<class T>
void case1(T ) { std::cout << "Case1 most generic\n"; }

template<class T>
void case1(T* ) { std::cout << "Case1 pointer generic\n"; }

template<>
void case1<>(int* ) { std::cout << "Case1 int pointer\n"; }
//(int *) is more special than T*

template<class T>
void case2(T ) { std::cout << "Case2 most generic\n"; }

template<>
void case2<>(int* ) { std::cout << "Case2 int pointer\n"; }

template<class T>
void case2(T* ) { std::cout << "Case2 pointer generic\n"; }
//T* is more special than int*?
/*
template <class T, bool isCloneable>
struct FactoryUtil {
  T* create(T* archetype) {
    if (isCloneable) {
      return archetype->clone();  // Type-checked for any type
    } else {
      return new T(*archetype);
    }
  }
}
//NOTE: This code does not compile! Compiler tries to evaluate both branches
    FactoryUtil<int, false> util;
    util.create(new int(5));

template_specialization.cpp: In instantiation of ‘T* FactoryUtil<T, isCloneable>::create(T*) [with T = int; bool isCloneable = false]’:
template_specialization.cpp:44:27:   required from here
template_specialization.cpp:28:31: error: request for member ‘clone’ in ‘* archetype’, which is of non-class type ‘int’
       return archetype->clone();  // Type-checked for any type
*/
//You can however use function overloading! One popular form of function overloading is tag dispatch
template <class T, bool isCloneable>
class FactoryUtil {
  T* create(T* archetype, std::true_type)  { return archetype->clone();}
  T* create(T* archetype, std::false_type) { return new T(*archetype); }
public:
  T* create(T* archetype) {
    using tag = std::integral_constant<bool, isCloneable>;
    return create(archetype, tag{});
  } 
};
//NOTE: Since isCloneable is known at compile time, the compiler can instantiate only the correct overload
/*Cannot use partial FUNCTION template specialization either!
template <class T, bool isCloneable = false>
T* create(T* archetype) {
return new T(*archetype);
}

template <class T>
T* create<T, true>(T* archetype) {
return archetype->clone();
}
NOTE: This code is illegal! The standard disallows partial template specialization of functions
*/
// BUT you  can also use partial CLASS template specialization!
template <class T, bool isCloneable= false>
struct FactoryUtil_2 {
  T* create(T* archetype) {  return new T(*archetype); }
};

template <class T>
struct FactoryUtil_2<T, true> {
  T* create(T* archetype) { return archetype->clone();}
};

void test_partial() {
    int* x = 0;

    case1(x);
    case2(x);
    //Case1 int pointer
    //Case2 pointer generic
    FactoryUtil<int, false> util;
    auto *p = util.create(new int(5));
    cout << "*p = " << *p << endl;
    delete p;

    FactoryUtil_2<int, false> util2;
    auto *p2 = util2.create(new int(2));
    cout << "*p2 = " << *p2 << endl;
    delete p2;
}

// Sum up a bunch of ints
template <int Val, int... Rest>
struct sum  {
  static constexpr int value = Val + sum<Rest...>::value;
};
// Terminate recursion:
template <int Last>
struct sum<Last> { // 'Rest' is an empty parameter pack here
  static constexpr int value = Last;
};


// Helper is_one_of<T, ...>::value checks if T is in a list of types (Us), A    
//variadic type bundle can contain 0   argument, The ... is a compile time loop
template <typename T, typename... Us>
//template <typename T>   has to have typename... Us, otherwise, compiler error : wrong number of template arguments
struct is_one_of : std::false_type { };
// Specialization to check first type and recurse
template <typename T, typename U, typename... Us>
struct is_one_of<T, U, Us...>  : std::integral_constant<bool,std::is_same<T, U>::value || is_one_of<T, Us...>::value> {
};

template <bool B>
using bool_constant = std::integral_constant<bool, B>;

// We would do this:
template <size_t N>
struct factorial_1 {
  static constexpr size_t value = N * factorial_1<N - 1>::value;
};

template <>
struct factorial_1<0> {
  static constexpr int value = 1;
};


// Also we want to use std::integral_constant
template <int N>
struct factorial_2 : std::integral_constant<int, N * factorial_2<N - 1>::value> { };

// Base case can be expressed as specialization
template <>
struct factorial_2<0> : std::integral_constant<int, 1> { };

template <size_t N>
struct factorial_3 {
 static constexpr size_t value = (N == 0) ? 1 : N * factorial_3<N - 1>::value;  // doesn't compile. N==0 as it HAS TO BE specialized
};

void print_bool(std::true_type) {
   std::cout << "true" << std::endl;
}

void print_bool(std::false_type) {
   std::cout << "false" << std::endl;
}
// Usage:
main() {
  test_partial();
  static_assert(sum<1, 2 ,3>::value == 6, "Look, I can sum digits!");
  //static_assert(sum<1, 2 ,3>::value == 7, "Look, you can't do the math! "); compiler error: template_specialization.cpp:111:3: error: static assertion failed: Look, you can't do the math! 
  static_assert(is_one_of<int, float, int, char>::value, "Yey!");
  cout << "factorial_1" << endl;
   print_bool(bool_constant<(factorial_1<0>::value > 20)>());
   print_bool(bool_constant<(factorial_1<1>::value > 20)>());
   print_bool(bool_constant<(factorial_1<2>::value > 20)>());
   print_bool(bool_constant<(factorial_1<3>::value > 20)>());
   print_bool(bool_constant<(factorial_1<4>::value > 20)>());
   print_bool(bool_constant<(factorial_1<5>::value > 20)>());
   print_bool(bool_constant<(factorial_1<6>::value > 20)>());
  cout << "factorial_2" << endl;
   print_bool(bool_constant<(factorial_2<0>::value > 20)>());
   print_bool(bool_constant<(factorial_2<1>::value > 20)>());
   print_bool(bool_constant<(factorial_2<2>::value > 20)>());
   print_bool(bool_constant<(factorial_2<3>::value > 20)>());
   print_bool(bool_constant<(factorial_2<4>::value > 20)>());
   print_bool(bool_constant<(factorial_2<5>::value > 20)>());
   print_bool(bool_constant<(factorial_2<6>::value > 20)>());

   //rint_bool(bool_constant<(factorial_2<-1>::value > 20)>()); compile error, over the depth

   //std::cout << factorial_3<1>::value  << std::endl; 
   // compile error:template_specialization.cpp:144:51:   recursively required from ‘constexpr const size_t factorial_3<0ul>::value’
 
}
