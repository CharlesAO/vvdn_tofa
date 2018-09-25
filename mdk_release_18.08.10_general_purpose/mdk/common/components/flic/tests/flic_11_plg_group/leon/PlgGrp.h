///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test super-plugin / sub-pipeline / plugin-group
///

//Include all plugs that make this super-plug
#include "PlgBuf.h"

#ifndef __PLG_GRP_H__
#define __PLG_GRP_H__

class PlgGrp : public PlgGroup
{
  private:
  //could have different plug types here ...
   PlgBuf plgA;
   PlgBuf plgB;
   PlgBuf plgC;

  public:
  //public IOs
   SReceiver<int> *in;
   MSender  <int> *out;

   void Create()
   {
     plgA.Create();
     plgB.Create();
     plgC.Create();

    //Public IOs/pads
     in  = &plgA.in;
     out = &plgC.out;
   }

   void AddTo(Pipeline *p)
   {
     p->Add(&plgA);
     p->Add(&plgB);
     p->Add(&plgC);

   //Internal links:
     plgA.out.Link(&plgB.in);
     plgB.out.Link(&plgC.in);
   }
};

#endif