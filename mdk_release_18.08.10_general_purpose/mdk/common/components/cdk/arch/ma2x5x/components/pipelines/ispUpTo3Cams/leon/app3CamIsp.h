/**************************************************************************************************

 @File         : app3CamIsp.c
 @Author       : MT
 @Brief        : Contains application interface For 3 isp in parallel
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/

#ifndef _APP3CAMISP_H_
#define _APP3CAMISP_H_

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "IcTypes.h"
#include "ipipe.h"

/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
// as lrt start, this function will be called. (Doc. 4.2)
void app3CamIspCbIcSetup(icCtrl *ctrl);

// application specific Tear Down
void app3CamIspCbIcTearDown(void);

//
icStatusCode app3CamIspSrcComit(icCtrl *ctrl);

//
void app3CamIspMain(void);

#endif // _APP3CAMISP_H_


