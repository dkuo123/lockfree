//#pragma once
#include <memory>
#include <thread>
#include <iostream>
#include <random>

using namespace std;

//multiple reader single writer first in first out fixed length ring buffer queue
//compatible with x86/x86_64 GCC
template<typename T,uint32_t N> class RnW1FifoFixed
{
private:
  static constexpr  uint32_t QSIZE= 2<<N;
  static constexpr  uint32_t MASK = QSIZE-1;
public:
  RnW1FifoFixed()
    :m_array{new T[QSIZE]},m_read{0},m_write{0}
  {
    static_assert(std::is_default_constructible<T>::value,"T does not have a default constructor.");
    static_assert(std::is_copy_assignable<T>::value,"T does not support copy assignment.");
    static_assert(N!=0,"N is too small.");
    cout << "Queue size = " << QSIZE << endl;
  }
  //one thread
  bool Write(const T& t)
  {
    //full
    if(m_write-m_read==QSIZE)
      return false;
    cout << "m_write = " << m_write << endl;
    m_array[m_write++&MASK] = t;

    //CPU does not reorder writes
    //prevent compiler from reordering writes
    //asm volatile("":::"memory");
    //m_write++;
    return true;
  }
  //multiple threads
  bool Read(T& t)
  {
    while(true)
    {
      //use a constant m_read each loop
      uint32_t read = m_read;
      //empty
      if(read==m_write)
        return false;
      t = m_array[read&MASK];
      if(__sync_bool_compare_and_swap(&m_read,read,read+1))
        return true;
    }
  }
private:
  std::unique_ptr<T[]> m_array;
  uint32_t m_read;
  uint32_t m_write;
};
/* unique_ptr can create array, but not shared_ptr usually.
std::shared_ptr<int> p(new int[10]); // ERROR, but compiles
std::shared_ptr<int> p(new int[10],
                      [](int* p) {
		          delete[] p;
			  });

https://herbsutter.com/gotw/_102/
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}
make_unique<int>(5);
*/

main() {
  auto N =  std::thread::hardware_concurrency();
  std::cout << "Number of threads = " << N << std::endl;
  RnW1FifoFixed<int, 10> dataPipe;

  int d[N-2] = {};
  for( auto i : d)
    cout << ",  " << i << endl;
  auto rt = [&](int i) {
    // random-number generator(1-5) (use i as seed to get different sequences)
    thread_local int ME = i;
    std::default_random_engine dre(i);
    std::uniform_int_distribution<int> id(1,5);
    cout << "starting reader " << ME << endl;
    while( d[ME] <= 5000 - N + 2) {
      if (dataPipe.Read(d[ME]))
        cout << "T " << ME << ": " << d[ME] << endl;
      std::this_thread::yield(); // hint to reschedule to the next thread
      this_thread::sleep_for(chrono::milliseconds(id(dre)));
    }
    cout << "exiting reader " << ME << endl;
  };

  for(int i = 0; i < N-2; ++i) {
    thread r(rt, i);
    /*
    thread r([&]() {
      // random-number generator(1-5) (use i as seed to get different sequences)
      thread_local int ME = i;
      std::default_random_engine dre(i);
      std::uniform_int_distribution<int> id(1,5);
      cout << "starting reader " << ME << endl;
      while( d[ME] < 5000 - N + 2) {
        if (dataPipe.Read(d[ME]))
          cout << "T " << ME << ": " << d[ME] << endl;
        std::this_thread::yield(); // hint to reschedule to the next thread
        this_thread::sleep_for(chrono::milliseconds(id(dre)));
      }
      cout << "exiting reader " << ME << endl;
    });
    */
    r.detach(); // run in background; not r.join() --- main thread will wait after the first thread starts
  }
  cout << "starting writer " << endl;
  thread w([&]() {
    std::default_random_engine dre(200);
    std::uniform_int_distribution<int> id(1,10);
    for (int i = 1; i <= 5000; ) {
      int loop = id(dre);
      for (int j = 0; j < loop && i <= 5000; )
        if(dataPipe.Write(i)) {
          ++i;
          ++j;
        }
      this_thread::sleep_for(chrono::milliseconds(1));
    }
    for(int i = 0; i < N-2; ) {
      if (d[i] > 5000 - N + 2) {
        cout << "DONE reader " << i <<  " = " << d[i] << endl;
        ++i;
      }
    }
    cout << "exiting writer" << endl;
    cout << "Press Enter to exit\n";

    cin.get();
  });

  w.join();

  cout << "done" << endl;
}
