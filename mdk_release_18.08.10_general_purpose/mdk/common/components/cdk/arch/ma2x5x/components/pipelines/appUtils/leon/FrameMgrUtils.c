/**************************************************************************************************

 @File         : FrameMgrUtils.h
 @Author       : MT
 @Brief        : Contain frames list create capability
 Date          : 30 - September - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/


/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "FrameMgrUtils.h"
#include "FrameMgrApi.h"


/**************************************************************************************************
 ~~~  Exported Functions Implementation
 **************************************************************************************************/

void initRawFramesList(FrameT* framesList, uint32_t width, uint32_t height, uint32_t dataSizeBy) {
    FrameT* frames = framesList;
    while (NULL != frames) {
        frames->stride[0]   = width * dataSizeBy;
        frames->height[0]   = height;
        frames->nPlanes     = 1;
        frames->status     = FRAMEMGR_EMPTY;
        frames->refcnt      = 0;
        frames->seqNo       = 0;
        frames->timestampNr = 0;
        frames = frames->next;
    }
}

void initNV12FramesList(FrameT* framesList, uint32_t width, uint32_t height, uint32_t dataSizeBy) {
    FrameT* frames = framesList;
    while (NULL != frames) {
        frames->stride[0]   = width * dataSizeBy;
        frames->height[0]   = height;
        frames->stride[1]   = (width) * dataSizeBy;
        frames->height[1]   = height>>1;
        frames->nPlanes     = 2;
        frames->status     = FRAMEMGR_EMPTY;
        frames->refcnt      = 0;
        frames->seqNo       = 0;
        frames->timestampNr = 0;
        frames = frames->next;
    }
}

FrameTFormatTypes FrmMgrUtilsGetRawFrm(uint32_t bpp, uint32_t packAllowed) {
    switch (bpp) {
    case 8:         return FRAME_T_FORMAT_RAW_8;
    case 10:        if(packAllowed) return FRAME_T_FORMAT_RAW_10_PACK;else return FRAME_T_FORMAT_RAW_10;
    case 12:        if(packAllowed) return FRAME_T_FORMAT_RAW_12_PACK;else return FRAME_T_FORMAT_RAW_12;
    case 14:        return FRAME_T_FORMAT_RAW_14;
    case 16:        return FRAME_T_FORMAT_RAW_16;
    default:        assert(0); return 0; // unsupported raw format
    }
}

void FrmMgrUtilsInitList(FrameT* framesList, icSize iSize, FrameTFormatTypes frmType) {
    FrameT* frames = framesList;
    uint32_t stride[4];
    uint32_t height[4];
    uint32_t nPlanes = 0;
    memset(stride, 0, sizeof(stride));
    memset(height, 0, sizeof(height));
    switch (frmType) {
    case FRAME_T_FORMAT_YUV420:
        stride[0] = iSize.w;
        stride[1] = iSize.w;
        height[0] = iSize.h;
        height[1] = iSize.h>>1;
        nPlanes = 2;
        break;
    case FRAME_T_FORMAT_YUV422P:
        stride[0] = iSize.w;
        stride[1] = iSize.w;
        height[0] = iSize.h;
        height[1] = iSize.h;
        nPlanes = 2;
        break;
    case FRAME_T_FORMAT_422I:
        stride[0] = iSize.w << 1;
        height[0] = iSize.h;
        nPlanes = 1;
        break;
    case FRAME_T_FORMAT_RAW_8:
        stride[0] = iSize.w;
        height[0] = iSize.h;
        nPlanes = 1;
        break;
    case FRAME_T_FORMAT_RAW_10:
        stride[0] = iSize.w * 2;
        height[0] = iSize.h;
        nPlanes = 1;
        break;
    case FRAME_T_FORMAT_RAW_10_PACK:
        stride[0] = (iSize.w * 10) >> 3;
        height[0] = iSize.h;
        nPlanes = 1;
        break;
    case FRAME_T_FORMAT_RAW_12:
        stride[0] = (iSize.w * 2);
        height[0] = iSize.h;
        nPlanes = 1;
        break;
    case FRAME_T_FORMAT_RAW_12_PACK:
        stride[0] = (iSize.w * 12) >> 3;
        height[0] = iSize.h;
        nPlanes = 1;
        break;
    case FRAME_T_FORMAT_RAW_14:
        stride[0] = (iSize.w * 2);
        height[0] = iSize.h;
        nPlanes = 1;
        break;
    default :
        assert(0); // invalid/unknown frame format initialization
        break;
    }
    while (NULL != frames) {
        uint32_t i;
        frames->type       = frmType;
        frames->refcnt     = 0;
        frames->status     = FRAMEMGR_EMPTY;
        for(i = 0; i < 4; i++) {
            frames->stride[i] = stride[i];
            frames->height[i] = height[i];
            frames->tSize[i]  = stride[i]*height[i];
        }
        frames->nPlanes     = nPlanes;
        frames->timestampNr = 0;
        frames->seqNo       = 0;
        frames->appSpecificData = NULL;
        frames = frames->next;
    }
}
