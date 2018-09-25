///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin (gender detect)
///

#ifndef __DET_GEN_H__
#define __DET_GEN_H__

#define GEN_STR "sexInfo"
#define GEN_VAL 1 ////0:male, 1:female

typedef struct {
  char szDes[8];
  int  gender;
} GenInfo;

//##########################################################
class PlgGen : public ThreadedPlugin
{
   public:
   SReceiver<CnnFramePtr> in;  //in
   MSender  <CnnFramePtr> out; //out

   //=========================================
   void Create(){
      in.Create(4);
      Add(&in , ".in");
      Add(&out, ".out");
   }

   //=========================================
   void * threadFunc(void *)
   {
       GenInfo   res;

       while(Alive())
       {
           CnnFramePtr  fIn;

           //1) Receive an frame/roi
           if(OK == in.Receive(&fIn))
           {
                CnnFrame *frm = fIn.ptr;

               //2)mimic detection
                FathomStart();
                FathomWait();
               //say wo got these results:
                sprintf(res.szDes, GEN_STR);
                res.gender = GEN_VAL;
               //append to description string
                frm->DescAdd((uint8_t*)&res, sizeof(res));

               //3)send results
                out.Send(&fIn);
           }
       }
       return NULL;
   }
};

#endif