///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Queue test: Push, Pop, TryPush, TryPop
///

#include <Flic.h>
#include <MsgBuf.h>
#include <UnitTestApi.h>

#define BUFF_SZ 8
#define TST_DATA(i) (100 + i)

struct MyMsg{
   uint32_t data;
};

MsgBuf<MyMsg> b;
MyMsg         m;

//##################################################
void PushPopTest(int tryFlag)
{
    uint32_t i;

  //1)Push till buff is full
    for(i=0; i<BUFF_SZ; i++){
      m.data = TST_DATA(i);
      unitTestAssert(OK == (tryFlag ? b.Push(NULL,&m) : b.TryPush(NULL,&m)));
    }
  //A new push should fail since queue is full
    unitTestAssert(b.Count() == BUFF_SZ);
    unitTestAssert(b.TryPush(NULL, &m) != OK);

  //2) Pop till buff gets empty
    for(i=0; i<BUFF_SZ; i++){
      unitTestAssert(OK == (tryFlag ? b.Pop(NULL,&m) : b.TryPop(NULL,&m)));
      unitTestAssert(m.data == TST_DATA(i));
    }
  //A new pop should fail since queue is empty
    unitTestAssert(b.Count()    ==  0);
    unitTestAssert(b.TryPop(NULL, &m) != OK);
}

//##################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    b.Create(BUFF_SZ);
  //Queue is barely created, should be empty
    unitTestAssert(b.TryPop(NULL, &m) != OK);
    unitTestAssert(b.Count () == 0);

    PushPopTest(0);
    PushPopTest(1);
    PushPopTest(0);

    unitTestFinalReport();
    exit(0);
}
