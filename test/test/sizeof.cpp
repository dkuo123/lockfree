#include <iostream>
using namespace std;
struct A{};
struct B{ virtual void foo() {} };

main() {
cout << "sizeof(char*) = " << sizeof(char*) << "\nsizeof(int*) = " << sizeof(int*) << endl;
cout << "sizeof(short) = " << sizeof(short) << "\nsizeof(int) = " << sizeof(int) << "\nsizeof(double) = " << sizeof(double) << "\nsizeof(long int) = " << sizeof(long int) << endl;
cout << "sizeof(A) = " << sizeof(A) << ", sizeof(B) = " << sizeof(B) << endl;
}
