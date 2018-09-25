/**************************************************************************************************

 @File         : mipiSendApi.h
 @Author       : MT
 @Brief        : Contains all mipi tx sending frame interaction
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/


#ifndef _MIPI_SEND_API_H_
#define _MIPI_SEND_API_H_

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "mv_types.h"
#include "IcTypes.h"
#include "sendOutApi.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 ~~~  Specific #defines
**************************************************************************************************/

/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void mipiSendCreate(MipiCfg_t *cfg);
void mipiSendInit(void);
void mipiSendSentFrame(SendOutElement_t *task);
void mipiSendFini(void);

#define TRANSPORT_create        mipiSendCreate
#define TRANSPORT_init          mipiSendInit
#define TRANSPORT_SendFrame     mipiSendSentFrame
#define TRANSPORT_Fini          mipiSendFini

#define TRANSPORT_PARAM         mipiCfg

#ifdef __cplusplus
}
#endif


#endif //_MIPI_SEND_API_H_
