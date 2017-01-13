#include <iostream>
using namespace std;

int output() {
 
  unsigned int foo = 0;
  unsigned short *foo_s = reinterpret_cast<unsigned short *>(&foo);
 *foo_s = 3;
 return foo; // optimizer may assume foo is not changed
}

int64_t par(int i, short u, char c, char* ptr) {
  int64_t res = i + u + (int)c + (int64_t)(ptr);
  return res;
}

main() {
  cout << output() << endl;
  char *p = new char('a');
  // char *p; this p can be any value, including zero when compiled with -O3 
  cout << (int)'c' << ", p = " << (void *)p << ", par = " << par(1,2,'c',p) << endl;

}
