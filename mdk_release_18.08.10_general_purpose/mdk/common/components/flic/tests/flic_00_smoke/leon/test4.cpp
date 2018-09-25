///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Overwrite-Oldest PUSH test
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueueOvr.h>
#include <GetClosest.h>
#include <UnitTestApi.h>

class MsgZ {
  public:
    uint32_t data;
};

//###############################################################
//Overwrite Oldest
//###############################################################
void test4()
{
    int rc;
    printf("sizeof (MsgZ) = %d\n", sizeof (MsgZ));
    printf("alignof(MsgZ) = %d\n", alignof(MsgZ));

    for(uint32_t i=0; i<5; i++)
    {
        printf("\n =========== RUN[%3ld]\n", i);
        MsgBuf      <MsgZ> buff;
        QueueCmdOvr <MsgZ> ex;

        buff.Create (4, QueueCmdOvr<MsgZ>::Inst());
        MsgZ msg;

      //buffer emtpty, pop should fail
        rc = buff.TryPop(NULL, &msg); unitTestAssert(rc != OK);

        msg.data = 100;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK);
        msg.data = 200;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK);
        msg.data = 300;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK);
        msg.data = 400;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK);
        msg.data = 500;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK);
        msg.data = 600;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK);
        msg.data = 700;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK);

        rc = buff.Pop(NULL, &msg); printf("%ld    (left=%d)\n", msg.data, buff.Count()); unitTestAssert(rc == OK);
        rc = buff.Pop(NULL, &msg); printf(" %ld   (left=%d)\n", msg.data, buff.Count()); unitTestAssert(rc == OK);
        rc = buff.Pop(NULL, &msg); printf("  %ld  (left=%d)\n", msg.data, buff.Count()); unitTestAssert(rc == OK);
        rc = buff.Pop(NULL, &msg); printf("   %ld (left=%d)\n", msg.data, buff.Count()); unitTestAssert(rc == OK);
      //rc = buff.Pop(NULL, &msg); //this would block the thread

      //buffer emtpty, pop should fail
        rc = buff.TryPop(NULL, &msg); unitTestAssert(rc == EAGAIN);

        buff.Destroy();
        buff.Delete ();
    }
}