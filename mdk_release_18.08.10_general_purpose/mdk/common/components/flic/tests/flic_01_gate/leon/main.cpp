///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test for gated receiver
///

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include "GateRec.h"
#include <UnitTestApi.h>

class MyMsg{
  public:
    uint32_t data;
};

MSender  <MyMsg> send;
PlgGate  <MyMsg> gate;
SReceiver<MyMsg> recv;//does msg buffering

//############################################
void test1()
{
    MyMsg m;
    m.data = 100; send.Send(&m);
    m.data = 200; send.Send(&m);
    m.data = 300; send.Send(&m);
    m.data = 400; send.Send(&m);

    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 100);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 200);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 300);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 400);
}

void test2()
{
    MyMsg m;
    m.data = 100; gate.en = true;  send.Send(&m);
    m.data = 200; gate.en = false; send.Send(&m);
    m.data = 300; gate.en = true;  send.Send(&m);
    m.data = 400; gate.en = false; send.Send(&m);
    m.data = 500; gate.en = true;  send.Send(&m);
    m.data = 600; gate.en = false; send.Send(&m);
    m.data = 700; gate.en = true;  send.Send(&m);

    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 100);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 300);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 500);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 700);
}

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    recv.Create(4);

    if(0){
      send.Link(&recv);
    }else{
      send.Link(&gate.in);
      gate.out.Link(&recv);
    }

    test1();
    test2();

    unitTestFinalReport();
    exit(0);
}
