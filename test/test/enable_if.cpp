#include <type_traits>
#include <iostream>
using namespace std;

/* Defaulted type param won't work - not part of signature, ambiguous if not specified
compiler error : template<class T> template<class U, class> static U check<T>::readVal()’ cannot be overloaded
 inline static U readVal();

template <class T>
struct check
{
template <class U =T,class = typename std::enable_if<std::is_same<U,int>::value>::type >
inline static U readVal() { cout << "int" << endl; return U(); }

  template <class U =T, class = typename std::enable_if<std::is_floating_point<U>::value>::type >
  inline static U readVal() { cout << "float point" << endl; return U();}
};
*/

int negate(int i) {
  return -i;
}

// negate(2) is deducted as const int& t. This code is invalid, of course, since int has no member named value_type. compiler error
template <typename T>
//typename T::value_type negate(const T& t, typename std::enable_if<!std::is_integral<T>::value>::type) {
typename T::value_type negate(const T& t,  std::enable_if_t<!std::is_integral<T>::value>* = nullptr) {
  return -t();
}

template <typename... Ts>
void type_str(Ts...) {

  cout  << __PRETTY_FUNCTION__ << " has num of parameters " << sizeof...(Ts) << endl;
}

main() {
  cout << " negate((int)2 = " << negate((int)2) << endl;
  //check<int> i;
  //i.readVal();
  //check<double>::readVal();
  auto x={1,2,3};
  type_str("Hello",0L, x);
}

//http://eli.thegreenplace.net/2014/sfinae-and-enable_if/
