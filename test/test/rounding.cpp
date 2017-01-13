#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

double roundToNearest(double x, double toNearest) {return (round(x/toNearest) * toNearest);}
double floorToNearest(double x, double toNearest=1.0) {return ( floor(x/toNearest) * toNearest);}
double ceilToNearest(double x, double toNearest=1.0) {return ( ceil(x/toNearest) * toNearest);}
main() {

//double base = 10.0+1/256;
double base = 96.60541009;

constexpr double step = 1.0/1024;
constexpr double NEAREST = 1.0/256;
cout << std::fixed << std::showpoint;
cout << std::setprecision(8);
cout << "base = " << base << ", step = " << step << ", NEAREST = " << NEAREST << endl;

for (int i = 0; i < 1025; ++i)
{
  double n = roundToNearest(base, NEAREST), f = floorToNearest(base, NEAREST), c = ceilToNearest(base, NEAREST);
  cout << "base = " << base <<  ", n = " << n << ", f = " << f << ", c = " << c 
	<< ", BUY diff = " << (n-f) *10000000 << ", SELL diff = " << (c-n) * 10000000 << endl;
  base += step;
}
}

