// Array Of Structures
struct Sphere
{
  float x;
  float y;
  float z;
  double r;
};
Sphere* AoS;

Memory layout (struct aligned on 8 bytes):
------------------------------------------------------------------
| x | y | z | r | pad | x | y | z | r | pad | x | y | z | r | pad
------------------------------------------------------------------

// Structure Of Arrays
struct SoA
{
  float* x;
  float* y;
  float* z;
  double* r;
  size_t size;
};

Memory layout:
------------------------------------------------------------------
| x | x | x ..| pad | y | y | y ..| pad | z | z | z ..| pad | r..
------------------------------------------------------------------
SSE is mostly geared toward vertical data representation (SoA) and processing, but generally speaking, the choice of Structure of Arrays (SoA) versus Array of Structures (AoS) for best performances depends on access patterns. 
◾AoS is probably the most natural one and easy to write. Fits OOP paradigms.
◾AoS has better data locality if all the members are accessed together.
◾SoA exposes more vectorization opportunities (vertical processing).
◾SoA often uses less memory because padding is only between arrays.
