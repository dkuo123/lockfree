#include <thread>
#include <atomic>
#include <cassert>
#include <vector>
#include <iostream>
 
auto data = int{0};
std::atomic<int> flag = {0};
 
void thread_1()
{
    data = 42;
    flag.store(1, std::memory_order_release);
}
 
void thread_2()
{
    int expected=1;
    // if flag == expected, flag = 2, return true; otherwise, expected = flag value, return false 
    while (!flag.compare_exchange_strong(expected, 2, std::memory_order_acq_rel)) {
	std::cout <<  expected << ", " << flag << std::endl;
        //expected = 1;
        //if comment out the above line, expected = 1;
        // expected will be = flag, which is 0; so the next CAS compare 0, 0, which is true and assign flag to 2.
        // or if thread 1 store 1 after both 0,0, it will change to 1, 1, then next round it will assign flag to 2.
        //
    }
}
 
void thread_3()
{
    while (flag.load(std::memory_order_acquire) < 2)
        ;
    std::cout <<  "flag = " << flag << ", data = " << data << std::endl;
    assert(data == 42); // will never fire? if T2 has expected = 1; then yes, otherwise, it can fire if 0, 1 -> 0, 0 -> 2, 2, by now if flag.store doesn't run yet, T1's data isn't exposed out yet, it's zero. if flag.store runs, then data = 42, even though flag = 1, not 2 as the while loop
}
 
int main()
{
    std::thread a(thread_1);
    std::thread b(thread_2);
    std::thread c(thread_3);
    a.join(); b.join(); c.join();
}

/*
 * g++ memory_order.cpp -pthread
 * for i in {1..100}; do /apps/homefs1/dguo/test/a.out; done

 flag = 2, data = 42

0, 0
flag = 2, data = 0
a.out: memory_order.cpp:38: void thread_3(): Assertion `data == 42' failed.
Aborted

0, 0
1, 1
flag = 2, data = 42

 */

