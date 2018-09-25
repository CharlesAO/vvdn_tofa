///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Master-Send -> Slave-Receive
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#include <Sender.h>
#include <Receiver.h>
#include <UnitTestApi.h>

class MsgU {
  public:
    uint32_t d;
};

//###############################################################
//Master-Send ---> Slave-Receive
//###############################################################
void test5()
{
   MsgU msg, m2;

   MSender  <MsgU> send;
   SReceiver<MsgU> recv;

   for(int i=0; i<10; i++)
   {
        recv.Create(4);
        send.Link(&recv);

        msg.d = 111; send.Send(&msg);
        msg.d = 222; send.Send(&msg);
        msg.d = 333; send.Send(&msg);

        unitTestAssert(OK==recv.Receive(&m2)); printf("__ Received %ld\n", m2.d);
        unitTestAssert(OK==recv.Receive(&m2)); printf("__  Received %ld\n", m2.d);
        unitTestAssert(OK==recv.Receive(&m2)); printf("__   Received %ld\n", m2.d);

        recv.Destroy();
        recv.Delete();
        send.Delete(); //nrecv = 0
   }
}