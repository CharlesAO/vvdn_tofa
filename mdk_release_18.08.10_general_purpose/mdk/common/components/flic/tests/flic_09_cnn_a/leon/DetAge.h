///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin (age detect)
///

#ifndef __DET_AGE_H__
#define __DET_AGE_H__

#define AGE_STR "ageInfo"
#define AGE_VAL 35

typedef struct {
  char szDes[8];
  int  age;
} AgeInfo;

//##########################################################
class PlgAge : public ThreadedPlugin
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
       AgeInfo   res;

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
                sprintf(res.szDes, AGE_STR);
                res.age = AGE_VAL;
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