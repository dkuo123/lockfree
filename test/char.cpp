#include <iostream>
using namespace std;
main() {
char c = 0x30;
for (int i =0; i < 10; ++i)
cout << "c = [" << (char) ( c+i) <<"]" << endl;
}
