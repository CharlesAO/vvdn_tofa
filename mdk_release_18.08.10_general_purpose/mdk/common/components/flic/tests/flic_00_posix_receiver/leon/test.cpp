///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Main test routines
///

#include <Flic.h>
#include <UnitTestApi.h>
#include "PmqRec.h"

#define Q_SIZE 4

//#########################################################################
struct MyMsg {
    uint32_t data;
};

//#########################################################################
//Push/Pop, TryPush/TryPop on a single buffer
void MqRecTest1()
{
    MyMsg    m;
    PmqRec<MyMsg> rec;

    rec.Create(Q_SIZE);

  //=================================================
  //Push/Pop test:
    for(uint32_t i=0; i<Q_SIZE; i++){
      m.data = i;
      unitTestAssert(OK == rec.Post(NULL, &m, &TS_INF));
    }
    for(uint32_t i=0; i<Q_SIZE; i++){
      unitTestAssert(OK == rec.Receive(&m));
      unitTestAssert(m.data == i);
    }

  //=================================================
  //TryPush/TryPop test:
    for(uint32_t i=0; i<Q_SIZE; i++){
      m.data = i;
      unitTestAssert(OK == rec.Post(NULL, &m, &TS_TRY));
    }
   //another push should fail
    unitTestAssert(OK != rec.Post(NULL, &m, &TS_TRY));

    for(uint32_t i=0; i<Q_SIZE; i++){
      unitTestAssert(OK == rec.TryReceive(&m));
      unitTestAssert(m.data == i);
    }
   //another pop should fail
    unitTestAssert(OK != rec.TryReceive(&m));

    rec.Destroy();
    printf("Posix MqRecTest1 done!\n");
}

//#########################################################################
//Push/Pop, on multiple buffers
void MqRecTest2()
{
    MyMsg    m;
    PmqRec<MyMsg> rec1;
    PmqRec<MyMsg> rec2;
    PmqRec<MyMsg> rec3;

    rec1.Create(Q_SIZE);
    rec2.Create(Q_SIZE);
    rec3.Create(Q_SIZE);

  //Different number of posts per queues
    m.data = 100; unitTestAssert(OK == rec1.Post(NULL, &m, &TS_INF));

    m.data = 200; unitTestAssert(OK == rec2.Post(NULL, &m, &TS_INF));
    m.data = 201; unitTestAssert(OK == rec2.Post(NULL, &m, &TS_INF));

    m.data = 300; unitTestAssert(OK == rec3.Post(NULL, &m, &TS_INF));
    m.data = 301; unitTestAssert(OK == rec3.Post(NULL, &m, &TS_INF));
    m.data = 302; unitTestAssert(OK == rec3.Post(NULL, &m, &TS_INF));

  //The corresponding pops
    unitTestAssert(OK == rec1.Receive(&m)); unitTestAssert(m.data == 100);

    unitTestAssert(OK == rec2.Receive(&m)); unitTestAssert(m.data == 200);
    unitTestAssert(OK == rec2.Receive(&m)); unitTestAssert(m.data == 201);

    unitTestAssert(OK == rec3.Receive(&m)); unitTestAssert(m.data == 300);
    unitTestAssert(OK == rec3.Receive(&m)); unitTestAssert(m.data == 301);
    unitTestAssert(OK == rec3.Receive(&m)); unitTestAssert(m.data == 302);

    rec1.Destroy();  rec1.Delete();
    rec2.Destroy();  rec2.Delete();
    rec3.Destroy();  rec3.Delete();
}

//#########################################################################
//Create-destroy cycle
//#########################################################################s
void MqRecTest3()
{
    for(int i=0; i<10000; i++)
    {
        MqRecTest2();
    }
    printf("Posix MqRecTest3 done!\n\n");
}