#include <iostream>

using namespace std;

struct NoPack {
  char Data1;
   //3-Bytes Added here.
   int Data2;
   unsigned short Data3;
   char Data4;
   //1-byte Added here.
   unsigned short Data5;
   // pad 2 bytes here

};

struct CharOnly
{
     unsigned char data1;
     unsigned char data2;
     unsigned char data3;
     unsigned char data4;
     unsigned char data5;
     //unsigned char data6;
// no padding since all are the same same 1
};

struct Char_Short
{
     unsigned char data1;
     unsigned char data2;
     unsigned char data3;
     unsigned char data4;
     unsigned char data5;
     //unsigned char data6;
     unsigned short Data7;
};

struct Char_Short2
{
     unsigned char data1;
     unsigned short Data7;
     unsigned char data2;
     unsigned char data3;
     unsigned char data4;
     unsigned char data5;
     //unsigned char data6;
};

struct Char_Int
{
     unsigned char data1;
     char data3;
     int data2;

};

struct Char_Int2
{
     unsigned char data1;
     int data2;
     char data3;
};

struct Packed {
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed)); //just pack, no need to align in 4 bytes


struct Packed1{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(1)));

struct Packed2{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(2)));

struct Packed4 {
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(4)));

struct Packed8{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(8)));

struct Packed16{
     char Data1;
     int Data2;
     unsigned short Data3;
     char Data4;
     unsigned short Data5;
} __attribute__((packed, aligned(16)));

struct CacheLine {
	int i;
} _attribute__(aligned(64)); // ensure each instance lands on a different cache line
// windows use __declspec(aligned(64)); 


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
}
