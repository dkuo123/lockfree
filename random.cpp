#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

using namespace std;

        class String {
        public:

                String(){};
                virtual ~String(){};

                static int findStringIndex(string & stringToFind, string * possibleMatches, size_t possibleMatchesSize);

                static size_t hash(const char *str) {
                        size_t out = 0;
                        while (*str) { out = out << 1 ^ *str++; }
                        return (out > 0 ? out : -out) % 982451653;
                }

};
               
static inline double stringToRnd(const string &key) {
                        size_t uintKey = String::hash(key.c_str());
                        srand(uintKey);
                        for (size_t idx = 1; idx < uintKey%15; idx++) rand();
                        return ((double)rand()) / ((double)RAND_MAX);
}

main() {
    ifstream file("rfq_id.txt");
    if(file.is_open())
    {
        istream_iterator<string> b(file), e;
	vector<string> v(b,e);
	for(int i = 0; i < v.size(); ++i)
		cout << v[i] << ", " << stringToRnd(v[i]) << endl;
    }

}
