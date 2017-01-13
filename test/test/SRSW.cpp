//only one thread as the Producer and only one thread as the Consumer
/*
#include <memory>
#include <thread>
#include <iostream>
#include <random>
#include <atomic>
#include <cassert>
*/

#include <bits/stdc++.h>  // this virtually has all the includes to shortcut

using namespace std;

// use default SC memory model

template<typename T, size_t N>
class CircularFifo{
private:
  static constexpr  uint32_t QSIZE= 2<<N;
  static constexpr  uint32_t MASK = QSIZE-1;
public:
  CircularFifo()
    :m_array{new T[QSIZE]},m_tail{0},m_head{0}
  {
    static_assert(std::is_default_constructible<T>::value,"T does not have a default constructor.");
    static_assert(std::is_copy_assignable<T>::value,"T does not support copy assignment.");
    static_assert(N!=0,"N is too small.");
    cout << "Queue size = " << QSIZE << endl;
  }

  // the idea is
  // if you're the only one to change a variable, you can load with relax, but store it with release to let other know.
  // if you're reading some variables potentially changed by others, you need acquire it.
  bool push(const T& item) {
    const auto head = m_head.load(memory_order_relaxed);
    const auto n_head = increment(head);
    if (n_head != m_tail.load(memory_order_acquire)) {
      m_array[head] = item;
      m_head.store(n_head, memory_order_release);
      return true;
    }

    return false;
  }

  bool pop(T& item) {
    const auto tail = m_tail.load(memory_order_relaxed);
    if (tail == m_head.load(memory_order_acquire))
      return false;

    const auto n_tail = increment(tail);
    item = m_array[tail];
    m_tail.store(n_tail, memory_order_release);
    return true;
  }

  bool isLockFree() const { return true; }
  size_t increment(size_t idx) const
  {
    //return (idx + 1) % QSIZE;
    return (idx+1) & MASK;
  }
private:
  //size_t increment(size_t idx) const;

  std::atomic<size_t>  m_tail;
  std::unique_ptr<T[]>  m_array;
  std::atomic<size_t>  m_head; // next element to push
};


main() {

  CircularFifo<int, 10> dataPipe;
  constexpr uint32_t N = 55000;

  thread r([&]() {
    // random-number generator(1-5) (use i as seed to get different sequences)
    std::default_random_engine dre(100);
    std::uniform_int_distribution<int> id(1,2);
    cout << "starting reader "  << endl;
    int last = 0;
    int current;
    for(auto i = 0; i < N; ) {
        if (dataPipe.pop(current)) {
          cout << current << endl;
          assert(current == last+1);
          ++last;
          ++i;
          this_thread::sleep_for(chrono::milliseconds(id(dre)));
        }
    }

    cout << "exiting reader " << endl;
  }
  );


  cout << "starting writer " << endl;
  thread w([&]() {
    std::default_random_engine dre(200);
    std::uniform_int_distribution<int> id(1,5);
    for (int i = 1; i <= N; ) {
      int loop = id(dre);
      for (int j = 0; j < loop && i <= N; )
        if(dataPipe.push(i)) {
          ++i;
          ++j;
        }
      this_thread::sleep_for(chrono::milliseconds(3));
    }
     cout << "exiting writer" << endl;
    cout << "Press Enter to exit\n";

    cin.get();
  });

  w.join();
  r.join();
  cout << "done" << endl;
}

