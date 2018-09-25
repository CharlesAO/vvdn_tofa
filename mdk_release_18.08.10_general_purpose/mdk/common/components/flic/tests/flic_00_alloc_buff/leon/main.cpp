///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Buffer Alloc/Destroy cycle
///

#include <Flic.h>
#include <MsgBuf.h>
#include <UnitTestApi.h>

#define N_RUNS  (100*1000)
#define B_SZ    8 //buffer size

//Some big data type
class MyMsg {
   public:
    uint32_t data;
    uint32_t buff[64];
};

//############################################
extern "C" void *POSIX_Init (void *)
{
    uint32_t      i,j;
    MyMsg         m;
    MsgBuf<MyMsg> b;

    unitTestInit();

    printf("Please wait ...\n");
    for(i=0; i<N_RUNS; i++)
    {
      //1)Create buffer (def queue behaviour)
        b.Create(B_SZ);

      //2)Fill buffer
        for(j=0; j<B_SZ; j++){
          m.data = j;
          b.Push(NULL, &m);
        }
        unitTestAssert(b.Count() == B_SZ);

      //3)Empty buffer
        for(j=0; j<B_SZ; j++){
           b.Pop(NULL, &m);
           assert(m.data == j);
        }
        unitTestAssert(b.Count() == 0);

      //4)Destory, delete
        b.Destroy();
        b.Delete ();
    }
    unitTestFinalReport();
    exit(0);
}
