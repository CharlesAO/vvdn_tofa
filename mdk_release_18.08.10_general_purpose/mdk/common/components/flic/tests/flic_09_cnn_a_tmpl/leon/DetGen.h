///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin (gender detect)
///

#ifndef __DET_GEN_H__
#define __DET_GEN_H__

template<typename T> class PlgGen : public ThreadedPlugin
{
   public:
   SReceiver<PoPtr<T>> in;  //in
   MSender  <PoPtr<T>> out; //out

   //=========================================
   void Create(){
      in.Create(4);
      Add(&in , ".in");
      Add(&out, ".out");
   }

   //=========================================
   void * threadFunc(void *)
   {
       while(Alive())
       {
           PoPtr<T>  fIn;

           //1) Receive an frame/roi
           if(OK == in.Receive(&fIn))
           {
                T *frm = fIn.ptr;

               //2)mimic detection
                FathomStart();
                FathomWait();
               //say wo got these results:
                frm->gender = TEST_GEN;

               //3)send results
                out.Send(&fIn);
           }
       }
       return NULL;
   }
};

#endif