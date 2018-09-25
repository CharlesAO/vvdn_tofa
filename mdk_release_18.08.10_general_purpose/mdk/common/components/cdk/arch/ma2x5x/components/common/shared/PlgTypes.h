/**************************************************************************************************

 @File         : PlgTypes.h
 @Author       : xx
 @Brief        : Contains Plug-in Basic typedef and defines
 Date          : 30 - April - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :
 **************************************************************************************************/


#ifndef _PLG_TYPES_H
#define _PLG_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 ~~~  Include standard types And General Plug-in used module
 **************************************************************************************************/
#include "IcTypes.h"


/**************************************************************************************************
 ~~~  Basic typdefs
 **************************************************************************************************//// after plugin.fini command, you need to check this status in order to have
/// confirmation that plug-in was finished.
typedef enum {
    PLG_STATS_IDLE    = 0,
    PLG_STATS_RUNNING = 1,
}PlgStatus;


typedef struct PlgTypeS {
    int32_t (*init)(FramePool *outputPools, int nOutputPools, void *pluginObj);
    int32_t (*fini)(void *pluginObj);
    volatile PlgStatus status; // !!! have to be initialized 0 at declaration
    FrameProducedCB *callbacks;
    int32_t (*resume)(void *pluginObj);
    int32_t (*trigger)(FrameT *frame, void *params, int (*callback)(int status), void *pluginObj);
}PlgType;

#ifdef __cplusplus
}
#endif

#endif  // _PLG_TYPES_H
