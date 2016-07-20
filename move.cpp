#include <iostream>
using namespace std;

class Class
{
    public:
    Class() { std::cerr << "ctor" << std::endl; }
    ~Class() { std::cerr <<"dtor" << std::endl; }

    Class(Class&)  { std::cerr << "copy ctor" << std::endl; }
    Class & operator=(const Class &)
    {
            std::cerr << "copy operator=" << std::endl;
            return *this;
    }

    Class(Class&&) { std::cerr << "move ctor" << std::endl;}
    Class & operator=(Class &&)
    {       
            std::cerr << "move operator="<< std::endl;
            return *this;
    }
 };

void foo1(Class c) {
  cout << "foo1(Class c) " << endl;
  Class local;
  local = c;
  cout << "exit foo1(Class c) " << endl;
}

void foo2(Class&& c) {
  cout << "foo2(Class&& c) " << endl;
  Class local;
  local = c;
  cout << "exit foo2(Class&& c) " << endl;
}


void foo3(Class&& c) {
  cout << "foo3(Class&& c) " << endl;
  Class local;
  local = move(c);
  cout << "exit foo3(Class&& c) " << endl;
}



int main(int, char**)
{
    Class object;
    Class && rvr = Class(); 
    object = rvr; // copy operator= 
    std::cerr << "again" << std::endl;
    object = std::move(rvr); // move operator=
    //rvr = object; // copy operator=
    
    cout << "passing to function" << endl;
    foo1(object); // copy ctor
                  // foo1(Class c)
                  // ctor
                  // copy operator=
                  // exit foo1(Class c)
                  // dtor
                  //dtor

    foo2(move(object)); // foo2(object) got compile error --- cannot bind ‘Class’ lvalue to ‘Class&&’
    /* foo2(Class&& c) // no move ctor
ctor
copy operator=
exit foo2(Class&& c)
dtor
     */

    foo3(move(object));
    /* foo3(Class&& c)
ctor
move operator=
exit foo3(Class&& c)
dtor
     */
    cout << "exit main" << endl;
    /*dtor
dtor
 */
}
