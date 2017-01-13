// http://eli.thegreenplace.net/2015/c-deleting-destructors-and-virtual-operator-delete/
#include <cstdio>

class Animal {
public:
  virtual void say() = 0;
  virtual ~Animal() {}
};

class Sheep : public Animal {
public:
  virtual void say() {
    printf("Sheep says baaaaa\n");
  }

  virtual ~Sheep() {
    printf("Sheep is dead\n");
  }

  void operator delete(void* p) {
    printf("Reclaiming Sheep storage from %p\n", p);
    ::operator delete(p);
  }
};

int main(int argc, char** argv) {
  Animal* ap = new Sheep;
  ap->say();
  delete ap;
/*
Sheep says baaaaa
Sheep is dead
Reclaiming Sheep storage from 0x1d27010

delete isn't virtual, in fact, it's static by C++ standard section 12.5:
*  Any deallocation function for a class X is a static member (even if not explicitly declared static). *
*  Since member allocation and deallocation functions are static they cannot be virtual.*

So how does this work, if operator delete is not virtual? Then answer is in a special destructor created for by the compiler: The entries for virtual destructors are actually pairs of entries. The first destructor, called the complete object destructor, performs the destruction without calling delete() on the object. The second destructor, called the deleting destructor, calls delete() after destroying the object.
*/
  {
    Sheep s;
  }
  // Sheep is dead

  {
     Sheep *s = new Sheep;
     delete s;
     /*Sheep is dead
       Reclaiming Sheep storage from 0x159f010*/
  }

  return 0;
}
