#include <vector>
#include <iostream>

using namespace std;
main() {
 vector<int> v0;
 cout <<"size of empty vector = " << sizeof(v0) << endl;
 v0.assign(20,0);
 cout <<" vector assign = " << sizeof(v0) << endl;
 vector<int> v{1,2,3,4,5,6,7,8,9,10, 11,12,13,14,15,16,17,18};
 cout << "int = " << sizeof(int) << ", ptr = " << sizeof(int *) << ", v = " << sizeof(v) << endl;
}


