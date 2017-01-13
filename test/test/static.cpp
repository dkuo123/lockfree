#include <tuple>
using namespace std;
struct my_class {
  // i & j are the same way for integeral type
  static const int i = 8;
  static constexpr int j = 8;
   
  // for non-int, this can't pass compiler
  //static const double d1 = 1.2; 
  // so it has to use 
  static const double d1;
  static constexpr double d2 = 1.2;
  
  static const int r1;
  // for calling function to decide value, it can't use constexpr
  //static constexpr int r2 = rand();
};
const double my_class::d1 = 1.2;
const int my_class::r1 = rand();

main () {
  int arr1[my_class::i];
  int arr2[my_class::j];
  arr1[0] = arr2[0];
}
