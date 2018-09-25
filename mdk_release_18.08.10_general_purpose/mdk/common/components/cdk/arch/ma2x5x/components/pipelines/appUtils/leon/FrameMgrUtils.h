/**************************************************************************************************

 @File         : FrameMgrUtils.h
 @Author       : MT
 @Brief        : Contain frames list create capability
 Date          : 30 - September - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/

#ifndef __FRAME_MGR_Utils_H__
#define __FRAME_MGR_Utils_H__


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "IcTypes.h"
/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Basic typdefs
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
//
void initRawFramesList(FrameT* framesList, uint32_t width, uint32_t height, uint32_t dataSizeBy);

//
void initNV12FramesList(FrameT* framesList, uint32_t width, uint32_t height, uint32_t dataSizeBy);

FrameTFormatTypes FrmMgrUtilsGetRawFrm(uint32_t bpp, uint32_t packAllowed);

void FrmMgrUtilsInitList(FrameT* framesList, icSize iSize, FrameTFormatTypes frmType);
#ifdef __cplusplus
}
#endif

#endif //__FRAME_MGR_Utils_H__
