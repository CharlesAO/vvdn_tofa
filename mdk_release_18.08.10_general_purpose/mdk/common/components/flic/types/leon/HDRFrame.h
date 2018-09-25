#ifndef __HDR_FRAME_H__
#define __HDR_FRAME_H__

#include <swcFrameTypes.h>
#include "Pool.h" //PoPtr
#include "PoolObj.h"

struct HDRFrame : public PoBuf
{
    frameBuffer longExp;  //
    frameBuffer shortExp; //
    uint32_t    categ;    // category : STILL, PREVIEW
    void *      userData; //
    uint32_t    instNo;   // Specify source instance
    int         seqNo;    // increments for each frame
    int64_t     ts;       // generation time
};

typedef PoPtr<HDRFrame> HDRFramePtr;

inline bool operator==(const HDRFramePtr& lhs, const HDRFramePtr& rhs)
{
   if((lhs.ptr        == rhs.ptr  ) &&
      (lhs.ref        == rhs.ref  ) &&
      (lhs.pool       == rhs.pool ) && // ...
      (lhs.ptr->seqNo == rhs.ptr->seqNo)) return true;
   else                                   return false;
}

#endif
