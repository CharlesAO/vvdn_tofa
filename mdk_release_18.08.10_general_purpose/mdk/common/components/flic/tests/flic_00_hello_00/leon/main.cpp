///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Hello World : pass 4 messages from Sender to Receiver
///

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <UnitTestApi.h>

class MyMsg{
  public:
    uint32_t data;
};

MSender  <MyMsg> send;
SReceiver<MyMsg> recv;//does msg buffering

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    recv.Create(4);
    send.Link(&recv);

    MyMsg m;
    m.data = 100; send.Send(&m);
    m.data = 200; send.Send(&m);
    m.data = 300; send.Send(&m);
    m.data = 400; send.Send(&m);

    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 100);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 200);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 300);
    unitTestAssert(OK == recv.Receive(&m)); unitTestAssert(m.data == 400);

    unitTestFinalReport();
    exit(0);
}
