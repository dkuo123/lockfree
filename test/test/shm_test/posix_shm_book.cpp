/*
 * http://man7.org/training/download/posix_shm_slides.pdf
||      orderBook  [-p <perms>] [-s <bytes>] [-c] [-x] [-r] [-t] [-w] <path>
||      -p <perms>  access mode to use when creating, default 0600
||      -s <bytes>  size of segment to map, default 64K
||      -c          use O_CREAT
||      -x          use O_EXCL
||      -r          use O_RDONLY, default is O_RDWR
||      -t          use O_TRUNC
||      -w          wait for keyboard input before exiting
||      <path>      the pathname of the queue, required
*/
#include <sys/mman.h>   /* shared memory and mmap() */
#include <sys/stat.h>        /* For mode constants */

#include <unistd.h>     /* for getopt() */
#include <errno.h>      /* errno and perror */
#include <fcntl.h>      /* O_flags */
#include <stdio.h>
#include <stdlib.h>

#include <bits/stdc++.h>  // this virtually has all the includes to shortcut
using namespace std;

struct Tick {
  int price;
  int size;
  Tick& operator ++() { ++price; ++size; return *this;}
};

static constexpr int VerifyCnt = 1000000;
struct OrderBook {
  static constexpr int LEVELS = 5;
  Tick bid[LEVELS], ask[LEVELS];
  OrderBook() : bid{{20, 20}, {19, 19}, {18,18}, {17,17}, {16,16}}, ask{{21, 21}, {22, 22}, {23, 23}, {24,24}, {25,25}} {}
  bool isGood() const {
    for(int i = 0; i < LEVELS; ++i) {
      if (bid[i].price +1+2*i != ask[i].price || bid[i].size + 1 +2*i != ask[i].size) {
        printf("bad in level %d -- <b %d %d, a %d %d\n", i, bid[i].price, bid[i].size, ask[i].price, ask[i].size);
        return false;
      }
    }
    return true;
  }

  void tick(int v) {
    for(int i = 0; i < LEVELS; ++i) {
      bid[i].price += v;
      ask[i].price += v;
      bid[i].size += v;
      ask[i].size += v;
    }
  }
};

struct SharedBook {
  atomic<bool> whichOne;
  OrderBook book[2];
  SharedBook() { book[1].tick(1); whichOne = false; }
  bool isGood() const {
    return book[whichOne?0:1].isGood();
  }
  void tick(int v) {
    book[whichOne?1:0].tick(v);
    whichOne = ~whichOne;
  }
  //SharedBook& SharedBook::operator=(SharedBook&&) = default;
};
int main(int argc, char **argv)
{
    int perms = 0600;           /* permissions */
    constexpr size_t size = sizeof(SharedBook);        /* segment size */
    int oflags = 0;             /* open flags receives -c, -x, -t */
    int ropt = 0;               /* -r option seen */
    int wopt = 0;               /* -w option seen */
    int shm_fd;                 /* file descriptor */
    int mprot = PROT_READ;      /* protection flags to mmap */
    int mflags = MAP_SHARED|MAP_LOCKED;    /* mmap flags */
    void *attach;               /* assigned memory adddress */
    char *path;                 /* ->first non-option argument */
    int c;
    while ( -1 != (c = getopt(argc,argv,"p:s:cxrtw")) )
    {
        switch (c)
        {
        case 'p': /* permissions */
            perms = (int) strtoul(optarg, NULL, 0);
            break;
        case 'c': /* use O_CREAT */
            oflags |= O_CREAT;
            break;
        case 'x': /* use O_EXCL */
            oflags |= O_EXCL;
            break;
        case 't': /* use O_TRUNC */
            oflags |= O_TRUNC;
            break;
        case 'r': /* use O_RDONLY */
            ropt = 1;
            break;
        default: /* unknown or missing argument */
            return -1;      
        } /* switch */
    } /* while */
    if (optind < argc)
        path = argv[optind]; /* first non-option argument */
    else
        { printf("Segment pathname required\n"); return -1; }
    if (0==ropt)
    { /* read-write access, reflect in mprot and mflags */
        oflags |= O_RDWR;
        mprot |= PROT_WRITE;
    }
    else
    { /* read-only access, mprot and mflags defaults ok */
        oflags |= O_RDONLY;
    }
    shm_fd = shm_open(path,oflags,perms);

    if (-1 != shm_fd)
    {
        if (mprot & PROT_WRITE) {
          ftruncate(shm_fd, size);
        }
        else {
          struct stat buf;
          fstat(shm_fd, &buf);
          printf("file size is %d, it should be %d\n", buf.st_size, size);
        }
        attach = mmap(NULL,size,mprot,mflags,shm_fd,(off_t)0);
        close(shm_fd); // fd is no longer needed
        if (attach != MAP_FAILED) /* mmap worked */
        {
            printf("Attached at 0x%lx, size %d\n",attach, size);
            if (mprot & PROT_WRITE)
            {
              //SharedBook* book = reinterpret_cast<SharedBook*> (attach);
              //book->SharedBook();
              //*book = SharedBook();
              SharedBook* book = new(attach) SharedBook();
              book->isGood();
              for (int i = 0; i < VerifyCnt; ++i) {
                std::default_random_engine dre(200);
                std::uniform_int_distribution<int> id(1,3);
                int wait = id(dre);
                this_thread::sleep_for(chrono::microseconds(wait));
                book->tick(2);
              }
              printf("writer is finished\n");
            }
            else {
              SharedBook* book = reinterpret_cast<SharedBook*> (attach);
              for (int i = 0; i < VerifyCnt; ++i) {
                std::default_random_engine dre(20);
                std::uniform_int_distribution<int> id(1,3);
                int wait = id(dre);
                this_thread::sleep_for(chrono::microseconds(wait));
                SharedBook lbook;
                memcpy(&lbook, book, size);
                lbook.isGood();
              }
              printf("reader is finished\n");
            }
            if (munmap(attach,size))
                perror("munmap()");
        }
        else
            perror("mmap()");
    }
    else {
        perror("shm_open()");
        shm_unlink(path);
    }
    return errno;
}

 

