///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Create/use/destroy cycle
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#include <UnitTestApi.h>

class MyMsg
{
  public: int data;
};

//###############################################################
//Basic create/use/destroy cycle
//###############################################################
void test1_a()
{
    printf("sizeof(time_t) = %d\n", sizeof(time_t));

    for(int i=0; i<10; i++)
    {
        MsgBuf  <MyMsg> buff;
        int rc;

        buff.Create(4);

        MyMsg msg;                                                             unitTestAssert(buff.Count() == 0);
        msg.data = 100;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 1);
        msg.data = 200;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 2);
        msg.data = 300;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 3);
        msg.data = 400;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 4);

        msg.data = 500;  rc = buff.TryPush(NULL, &msg); unitTestAssert(rc != OK);

        rc = buff.Pop(NULL, &msg); printf("%d\n", msg.data);    unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 3);
        rc = buff.Pop(NULL, &msg); printf(" %d\n", msg.data);   unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 2);
        rc = buff.Pop(NULL, &msg); printf("  %d\n", msg.data);  unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 1);
        rc = buff.Pop(NULL, &msg); printf("   %d\n", msg.data); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 0);

        rc = buff.TryPop(NULL, &msg);                           unitTestAssert(rc != OK);

        buff.Destroy();
        buff.Delete ();
    }
}

//###############################################################
void test1_b() //primitive type msg buffer
//###############################################################
{
   for(int i=0; i<10; i++)
    {
        MsgBuf <int> buff;
        int rc;

        buff.Create(4);

        int msg;                                                          unitTestAssert(buff.Count() == 0);
        msg = 500;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 1);
        msg = 600;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 2);
        msg = 700;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 3);
        msg = 800;  rc = buff.Push(NULL, &msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 4);

        msg = 900;  rc = buff.TryPush(NULL, &msg); unitTestAssert(rc != OK);

        rc = buff.Pop(NULL, &msg); printf("%d\n", msg);    unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 3);
        rc = buff.Pop(NULL, &msg); printf(" %d\n", msg);   unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 2);
        rc = buff.Pop(NULL, &msg); printf("  %d\n", msg);  unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 1);
        rc = buff.Pop(NULL, &msg); printf("   %d\n", msg); unitTestAssert(rc == OK); unitTestAssert(buff.Count() == 0);

        rc = buff.TryPop(NULL, &msg);                      unitTestAssert(rc != OK);

        buff.Destroy();
        buff.Delete ();
    }
}
//###############################################################
void test1()
{
    test1_a();
    test1_b();
}