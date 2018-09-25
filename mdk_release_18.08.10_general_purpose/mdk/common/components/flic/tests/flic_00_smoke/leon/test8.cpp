///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Deep copy via copy-constructor
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#include <Sender.h>
#include <Receiver.h>
#include <MemAllocator.h>
#include <UnitTestApi.h>
#include "ImgFrame.h"


#define ASSERT unitTestAssert
#define N 64

//###############################################################
// message with dynamic alloc/dealloc
// + no default argument-less constructor
class MyT
{
 public:
  uint32_t *data;
  uint32_t sz;

  //IMPORTANT: deep copy required !!!
  MyT(const MyT& other){
     sz = other.sz;
     if(sz){
       data = new uint32_t [sz];
       memcpy(data, other.data, sz * sizeof(data[0]));
     }
  }

  MyT(uint32_t n){
    sz   = n;
    if(n) data = new uint32_t [n];
    else  data = NULL;
  }

  MyT& operator = (const MyT& other)
  {
    assert(this->sz == other.sz);
    if(other.data){
      memcpy(data, other.data, sz * sizeof(data[0]));
    }
    return *this;
  }

  ~MyT(){
    if(data){
      delete[] data;
    }
  }
};

//###############################################################
// alloc/dealloc in constructor/destructor
// no default constructor available
//###############################################################
void test8()
{
    int rc;

    for(uint32_t i=0; i<100; i++)
    {
      printf("__test8__ (%ld)\n", i);
      MsgBuf<MyT> buff;
      buff.Create(4);
      MyT msg(N);

      for(uint32_t j=0; j<3*1500; j+=3)
      {
          msg.data[0] = j+0; rc = buff.Push(NULL, &msg); ASSERT(rc == OK);
          msg.data[0] = j+1; rc = buff.Push(NULL, &msg); ASSERT(rc == OK);
          msg.data[0] = j+2; rc = buff.Push(NULL, &msg); ASSERT(rc == OK);

          msg.data[0] = 0xABBA0000;

          rc = buff.Pop(NULL, &msg); ASSERT(rc == OK); ASSERT(msg.data[0] == j+0);
          rc = buff.Pop(NULL, &msg); ASSERT(rc == OK); ASSERT(msg.data[0] == j+1);
          rc = buff.Pop(NULL, &msg); ASSERT(rc == OK); ASSERT(msg.data[0] == j+2);
      }

      buff.Destroy();
      buff.Delete ();
    }
}
