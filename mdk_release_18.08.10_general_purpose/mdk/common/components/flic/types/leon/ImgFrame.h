#ifndef __IMG_FRAME_H__
#define __IMG_FRAME_H__

#include <swcFrameTypes.h>
#include "Pool.h" //PoPtr
#include "PoolObj.h"

struct ImgFrame : public PoBuf
{
    frameBuffer fb;       //
    uint32_t    categ;    // category : STILL, PREVIEW
    void *      userData; //
    uint32_t    instNo;   // Specify source instance
    int         seqNo;    // increments for each frame
    int64_t     ts;       // generation time
};

typedef PoPtr<ImgFrame> ImgFramePtr;

inline bool operator==(const ImgFramePtr& lhs, const ImgFramePtr& rhs)
{
   if((lhs.ptr        == rhs.ptr  ) &&
      (lhs.ref        == rhs.ref  ) &&
      (lhs.pool       == rhs.pool ) && // ...
      (lhs.ptr->seqNo == rhs.ptr->seqNo)) return true;
   else                                   return false;
}

#endif
