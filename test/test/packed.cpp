#include <iostream>

using namespace std;

struct CharOnly
{
     unsigned char data1;
     unsigned char data2;
     unsigned char data3;
     unsigned char data4;
     unsigned char data5;
     //unsigned char data6;
// no padding since all are the same same 1
}; //sizeof(CharOnly) = 5

struct Char_Short
{
     unsigned char data1;
     unsigned char data2;
     unsigned char data3;
     unsigned char data4;
     unsigned char data5;
     //unsigned char data6;
     unsigned short Data7;
}; // sizeof(Char_Short) = 8

struct Char_Short2
{
     unsigned char data1;
     unsigned short Data7;
     unsigned char data2;
     unsigned char data3;
     unsigned char data4;
     unsigned char data5;
     //unsigned char data6;
}; // sizeof(Char_Short2) = 8

struct Char_Int
{
     unsigned char data1;
     char data3;
     int data2;

}; //sizeof(Char_Int) = 8

struct Char_Int2
{
     unsigned char data1;
     int data2;
     char data3;
}; // sizeof(Char_Int2) = 12

struct NoPack {
  char Data1;
   //3-Bytes Added here.
   int Data2;
   unsigned short Data3;
   char Data4;
   //1-byte Added here.
   unsigned short Data5;
   // pad 2 bytes here

}; //sizeof(NoPack) = 16

struct Packed {
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed)); //just pack, no need to align in 4 bytes
// sizeof(Packed) = 10

struct Packed1{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(1)));
//sizeof(Packed1) = 10, same as not habing aligned(1)

struct Packed2{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(2)));
//sizeof(Packed2) = 10 because 10%2 = 0
struct Packed4 {
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(4)));
// sizeof(Packed4) = 12, 10%4 != 0, so pad it up to make it 4 times
struct Packed8{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(8)));
// sizeof(Packed8) = 16,  next number over 10 which is multiple of 8
struct Packed16{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(16)));
//sizeof(Packed16) = 16
struct Packed32{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(32)));
// sizeof(Packed32) = 32
struct CacheLine {
	int i;
} __attribute__((aligned(64))); // ensure each instance lands on a different cache line
// windows use __declspec(aligned(64)); 
//sizeof(CacheLine) = 64

/* to pack all the data structures using default 4 types
#pragma pack(push, 1)

//Structure 1
......

//Structure 2
......

#pragma pack(pop)
*/

main() {
  cout << "sizeof(CharOnly) = " << sizeof(CharOnly) << endl; // 5
  cout << "sizeof(Char_Short) = " << sizeof(Char_Short) << endl; //8
  cout << "sizeof(Char_Short2) = " << sizeof(Char_Short2) << endl; //8, add data6 to make it 10 ???

  cout << "sizeof(Char_Int) = " << sizeof(Char_Int) << endl; // 8
  cout << "sizeof(Char_Int2) = " << sizeof(Char_Int2) << endl; // 12

  cout << "sizeof(NoPack) = " << sizeof(NoPack) << endl; // 16
  cout << "sizeof(Packed) = " << sizeof(Packed) << endl;  // 10
  cout << "sizeof(Packed1) = " << sizeof(Packed1) << endl;  // 10
  cout << "sizeof(Packed2) = " << sizeof(Packed2) << endl;  //10
  cout << "sizeof(Packed4) = " << sizeof(Packed4) << endl;  // 12
  cout << "sizeof(Packed8) = " << sizeof(Packed8) << endl;  // 16
  cout << "sizeof(Packed16) = " << sizeof(Packed16) << endl; //16
  cout << "sizeof(Packed32) = " << sizeof(Packed32) << endl; //32
  cout << "sizeof(CacheLine) = " << sizeof(CacheLine) << endl; //64
}
