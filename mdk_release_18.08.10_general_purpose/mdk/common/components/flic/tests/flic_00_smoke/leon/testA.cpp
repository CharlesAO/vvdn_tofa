///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Checking Send error codes
///

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <UnitTestApi.h>

#define ONE_MS (1000*1000) //[ns]

class Msg {
  public:
    uint32_t d;
};

//###############################################################
//Timed MSend/SReceive test : check Sender err codes
//###############################################################
void testA()
{
   Msg m;
   timespec timeOut {.tv_sec  = 0,
                     .tv_nsec = 10 * ONE_MS};

   for(int i=0; i<20; i++)
   {
     printf("test A %2d\n", i);
     MSender  <Msg> send;
     SReceiver<Msg> recv[3];

     recv[0].Create(4); //can hold 4 x msg
     recv[1].Create(1); //can hold 1 x msg
     recv[2].Create(1); //can hold 1 x msg

     send.Link(&recv[0]);
     send.Link(&recv[1]);
     send.Link(&recv[2]);

   //=========================================
   //1st push : all should succeed
     m.d = 10; assert(OK == send.Send(&m));

     assert(send.errNo[0] == OK);
     assert(send.errNo[1] == OK);
     assert(send.errNo[2] == OK);
     assert(!send.HasErr(EINVAL));

   //=========================================
   //2nd push: should succeed on 1st consumer,
   // the rest should timeout due to buffer full
     m.d = 11; assert(OK != send.Send(&m, &timeOut));

     assert(send.errNo[0] == OK);
     assert(send.errNo[1] == ETIMEDOUT);
     assert(send.errNo[2] == ETIMEDOUT);

   //=========================================
   //3rd push: breaking last consumer before last push
     recv[2].Destroy(); //kills internal sem
   //1st consumer: should be OK
   //2nd consumer: should timeout
   //3rd consumer: should be invalid
     m.d = 12; assert(OK != send.Send(&m, &timeOut));

     assert(send.errNo[0] == OK);
     assert(send.errNo[1] == ETIMEDOUT);
     assert(send.errNo[2] == EINVAL);
     assert(send.HasErr(EINVAL));

       recv[0].Destroy();   recv[0].Delete();
       recv[1].Destroy();   recv[1].Delete();
     /*recv[2].Destroy();*/ recv[2].Delete();
   }
}
