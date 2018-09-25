/**************************************************************************************************

 @File         : ImgTypes.h
 @Brief        : Contains All projects specific image related types definition
 Date          : 30 - April - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :


 **************************************************************************************************/

#ifndef __IMGTYPES_H_
#define __IMGTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 ~~~  Include standard types
 **************************************************************************************************/
#include <stdint.h> //Myriad and PC

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

/**************************************************************************************************
 ~~~  Basic project types definition
 **************************************************************************************************/

typedef struct {
    uint32_t    w;
    uint32_t    h;
} ImgSize;

typedef struct {
    int32_t     x1;
    int32_t     y1;
    int32_t     x2;
    int32_t     y2;
} ImgRect;


typedef enum {
    FRAME_FMT_422I        ,
    FRAME_FMT_NV12        ,
    FRAME_FMT_NV21        ,
    FRAME_FMT_RAW_10_PACK ,
    FRAME_FMT_BINARY      ,  //  metadata
    FRAME_FMT_RAW_16      ,   // RAW16 packet
    FRAME_FMT_NV12_Y      ,
    FRAME_FMT_NV12_UV     ,
    FRAME_FMT_YUV420      ,
    FRAME_FMT_RAW_8       ,
    FRAME_FMT_RAW_10      ,
    FRAME_FMT_RAW_12      ,
    FRAME_FMT_RAW_12_PACK ,
    FRAME_FMT_RAW_14      ,
    FRAME_FMT_RGB_888     ,
    FRAME_FMT_RGB_101010  ,
    FRAME_FMT_RGB_121212  ,
    FRAME_FMT_YUV422P     ,
    FRAME_FMT_RAW_32      ,
    FRAME_FMT_USER_01     , // user specific structure
    FRAME_FMT_USER_02     , // user specific structure
    FRAME_FMT_USER_03     , // user specific structure
    FRAME_FMT_YUV444      ,
    FRAME_FMT_YUV420P     ,
    FRAME_FMT_NONE        , //TODO: this have to be 0, backward compatibility force this
    FRAME_FMT_MAX
} ImgFrmFmt;


typedef enum {
    FRAME_TYPE_PREVIEW       ,
    FRAME_TYPE_STILL         ,
    FRAME_TYPE_STILL_RAW     ,
    FRAME_TYPE_METADATA_PRV  ,
    FRAME_TYPE_METADATA_STILL,
    FRAME_TYPE_PREVIEW_RAW   ,
    FRAME_TYPE_IQ_DEBUG_DATA ,
    FRAME_TYPE_USER_01       ,
    FRAME_TYPE_USER_02       ,
    FRAME_TYPE_USER_03       ,
	FRAME_TYPE_NONE          , //TODO: this have to be 0, backward compatibility force this
    FRAME_TYPE_MAX           ,
} ImgFrmType;

#ifdef __cplusplus
}
#endif

#endif /* __IMGTYPES_H_ */
