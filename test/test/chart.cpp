#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

double roundToNearest(double x, double toNearest) {return (round(x/toNearest) * toNearest);}
main() {

double base = 0.0;

double step = 1.0/256;
cout << std::fixed << std::showpoint;
cout << std::setprecision(8);
cout << "price  \t,step" <<  endl;

for (int i = 0; i < 32; ++i)
for (int j = 0; j < 8; ++j)
{
  //double n = roundToNearest(base, NEAREST), f = floorToNearest(base, NEAREST), c = ceilToNearest(base, NEAREST);
  cout << base <<  "\t," << i << " + " << j << endl; 
  base += step;
}
}

