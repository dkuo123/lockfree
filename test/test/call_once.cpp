#include <iostream>
#include <thread>
#include <mutex>
 
std::once_flag flag1, flag2;
 
void simple_do_once(int i)
{
  std::call_once(flag1, [i](){ std::cout << "Simple example: called once from thread " << i << "\n"; });
}
 
void may_throw_function(bool do_throw)
{
  if (do_throw) {
    std::cout << "throw: call_once will retry\n"; // this may appear more than once
    throw std::exception();
  }
  std::cout << "Didn't throw, call_once will not attempt again\n"; // guaranteed once
}
 
void do_once(bool do_throw)
{
  try {
    std::call_once(flag2, may_throw_function, do_throw);
  }
  catch (...) {
  }
}
 
int main()
{
  std::thread st1(simple_do_once, 4);
  std::thread st2(simple_do_once, 3);
  std::thread st3(simple_do_once,1 );
  std::thread st4(simple_do_once,2);
    st1.join();
    st2.join();
    st3.join();
    st4.join();
 
    std::thread t1(do_once, true);
    std::thread t2(do_once, true);
    std::thread t3(do_once, false);
    std::thread t4(do_once, true);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
/*
Simple example: called once from thread 4
throw: call_once will retry
throw: call_once will retry
Didn't throw, call_once will not attempt again
*/
