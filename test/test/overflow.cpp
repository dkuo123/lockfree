#include <iostream>
#include <cstring>

using namespace std;

main() {
uint16_t val(60000);
int16_t val1, val2;
val1 = val;
memcpy(&val2, &val, sizeof val);
cout << val << ", " << val1 << ", " << val2 << endl;
// 60000, -5536
// Same bits, signed vs. unsigned interpretation
 
}
