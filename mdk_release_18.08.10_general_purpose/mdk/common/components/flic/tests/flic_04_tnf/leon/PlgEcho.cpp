///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <string.h>
#include <ImgFrame.h>
#include <UnitTestApi.h>

//##################################################################
//dummy test data generator
uint32_t dataGen(uint32_t n)
{
    uint32_t v = 0;
    uint32_t u = 2;

    for(uint32_t i=0; i<n; i++)
    {
       v = v + u;
       u = u + 1;
    }
    return (uint8_t)v;
}

//##################################################################
void PlgEchoSend::Create()
{
    Add(&in);Add(&out);
}
//=============================================
void * PlgEchoSend::threadFunc(void *)
{
    uint8_t k = 0;

    while(Alive())
    {
      ImgFramePtr fMsg;//from mem pool
      rtems_task_wake_after(500);

     //1)get a frame from mempool
      if(OK == in.Receive(&fMsg))
      {
        //2)change frame data (as Camera would)
         memset(fMsg.ptr->base, dataGen(k++), fMsg.ptr->size);

        //printf("Buff = %x, Data = %d\n", (int)ptr.base, ((uint8_t*)ptr.base)[0]);
        //3)Send data
         out.Send(&fMsg);
      }
    } //'fMsg' automatically released here
    return NULL;
}

//##################################################################
void PlgEchoRecv::Create()
{
    in.Create(8);
    Add(&in);
}
//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    uint32_t exp, k = 0;

    while(Alive())
    {
        ImgFramePtr fMsg;

        if(OK == in.Receive(&fMsg))
        {
          printf("Data = %d\n", ((uint8_t*)fMsg.ptr->base)[0]);

         //reconstruct TNF expected output
          switch(k){
            case  0: exp =  dataGen(k  );                                break;
            case  1: exp = (dataGen(k-1) + dataGen(k  )             )/2; break;
            default: exp = (dataGen(k-2) + dataGen(k-1) + dataGen(k))/3; break;
          }
          k++;
          unitTestAssert(((uint8_t*)fMsg.ptr->base)[0] == (uint8_t)exp);
        }
    }//'fMsg' automatically released here
    return NULL;
}