#ifndef _APP_ISP_FROM_RX_H_
#define _APP_ISP_FROM_RX_H_

#include "IcTypes.h"
#include "ipipe.h"

void         appRxIspCbIcSetup(icCtrl *ctrl);
void         appRxIspCbIcTearDown(void);
icStatusCode appRxIspSrcComit(icCtrl *ctrl);
void         appRxIspMain();

#endif


