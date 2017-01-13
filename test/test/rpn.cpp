#include <iostream>
#include <string.h> 
#include <stack>
#include <sstream>
#include <stdlib.h>

using namespace std ;

inline bool isOperator(char c) { 
  return c == '+' || c == '-' || c == '*' || c == '/';
}

enum ERROR {
  OK,
  TooFewNumber,
  TooManyNumber
};

int main()
{
  stack<float> nums;
  string line,token;
  ERROR ret = OK;

  //while (getline(cin,line)) { // parse each line
  getline(cin,line);
  istringstream is(line); 

  while(is>>token) {
    cout << "processing " << token << ", nums.size() = " << nums.size() << endl;
    if (isOperator(token[0])) {
      if (nums.size() < 2) {
	ret = TooFewNumber;
	break;
      }  
      float a  = nums.top();
      nums.pop();
      float& b = nums.top();
      switch(token[0]) {
      case '+': b += a; break;
      case '-': b -= a; break;
      case '*': b *= a; break;
      case '/': b /= a; break;
      }		
    }
    else {
      // using boost::lexical_cast;
      //  using boost::bad_lexical_cast;
      
      // nums.push(strtof(str1.c_str(),NULL));  
      // assuming it's float
      nums.push(atof(token.c_str()));
    }
  }
  if (nums.size() > 1)
    ret = TooManyNumber;
  if ( ret == OK)
    cout<< "The result of the expression is: " << nums.top() << endl;
  else
    cout << "input line is in wrong format " << ret << endl;
} 
