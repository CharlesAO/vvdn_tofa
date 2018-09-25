/**************************************************************************************************

 @File         : FrameTypes.h
 @Brief        : Contains Image or binary common recognize frame description
 Date          : 30 - April - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :


 **************************************************************************************************/

#ifndef __FRAMETYPES_H_
#define __FRAMETYPES_H_

#include "ImgTypes.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

#ifndef IMG_MAX_TS
#define IMG_MAX_TS 4 // Maximum history timestamp associated with a frame
#endif
#ifndef IMG_MAX_PLANES
#define IMG_MAX_PLANES 4 // Maximum history timestamp associated with a frame
#endif

// MsgBuf data description structure
typedef struct FrameS {
    ImgFrmFmt  fmt; //
    ImgFrmType type; //
    void *     fbPtr [IMG_MAX_PLANES];
    uint32_t   stride[IMG_MAX_PLANES];
    uint32_t   width [IMG_MAX_PLANES];
    uint32_t   height[IMG_MAX_PLANES];
    uint32_t   tSize [IMG_MAX_PLANES];
    uint32_t   nPlanes;
    uint64_t   ts     [IMG_MAX_TS];
    uint32_t   tsEvent[IMG_MAX_TS]; // app specific events associated with timestamps
    uint32_t   tsNr;
    uint32_t   seqNo; // index for the frame
    void       *userData; // associated app specific information with this frame
    uint32_t   instId; // instance associated with this frame
} FrameT;


#endif /* __FRAMETYPES_H_ */
