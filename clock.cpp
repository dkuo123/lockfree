// g++ clock.cpp -lrt  // need lib rt 
//for ((i=1; i < 10; i++)); do ./a.out; done
/*
0 s, 123 ns   // cpu isn't busy and "relax". after the first call, it wake-up and the time elapse in 2 call is much shorter.
0 s, 30 ns
0 s, 15 ns
0 s, 15 ns
0 s, 15 ns
0 s, 22 ns
0 s, 15 ns
0 s, 23 ns
0 s, 15 ns
0 s, 22 ns
0 s, 15 ns
0 s, 23 ns
0 s, 15 ns
0 s, 23 ns
0 s, 15 ns
0 s, 22 ns
0 s, 15 ns
0 s, 15 ns
0 s, 15 ns
0 s, 22 ns
0 s, 104 ns    // when call a.out again
0 s, 32 ns
0 s, 27 ns
...

 see detail on http://btorpey.github.io/blog/2014/02/18/clock-sources-in-linux/
The other thing that happens when the system boots is that the TSC (Time Stamp Counter) starts running. The TSC is a register counter that is also driven from a crystal oscillator – the same oscillator that is used to generate the clock pulses that drive the CPU(s). As such it runs at the frequency of the CPU, so for instance a 2GHz clock will tick twice per nanosecond.
TSC is generally the preferred clock source, given its accuracy and relatively low overhead
*/
#include <stdio.h>
#include <time.h>
#include <unistd.h>
//#define TEST_CID  CLOCK_PROCESS_CPUTIME_ID  // this one takes a lot of more time than CLOCK_REALTIME
//#define TEST_CID  CLOCK_REALTIME
#define TEST_CID CLOCK_MONOTONIC
int main(int argc, char **argv, char **arge) {
  struct timespec tps, tpe;
  {
    for(int i = 0; i < 20; ++i) {
      if ((clock_gettime(TEST_CID, &tps) != 0)  // CLOCK_REALTIME, CLOCK_PROCESS_CPUTIME_ID  
	  || (clock_gettime(TEST_CID, &tpe) != 0)) {
	perror("clock_gettime");
	return -1;
      }

      printf("%lu s, %lu ns\n", tpe.tv_sec-tps.tv_sec,
	     tpe.tv_nsec-tps.tv_nsec);
    }
  }
  // spin time
  sleep(2); // sleep bring back the first clock_gettime() latency
  printf("Spinning ...\n");
  {
    for(int i = 0; i < 20; ++i) {
      clock_gettime(TEST_CID, &tps);
      __asm__ __volatile__ ("nop"); // they don't seem to take any time statistically
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");
      __asm__ __volatile__ ("nop");

      clock_gettime(TEST_CID, &tpe);

      printf("%lu s, %lu ns\n", tpe.tv_sec-tps.tv_sec,
	     tpe.tv_nsec-tps.tv_nsec);
    }
  }
  return 0;
}
