#ifndef ISPUSERCONTROLSDATA_H_
#define ISPUSERCONTROLSDATA_H_

#define ISP_USRCTRL_BRIGHTNESS  0x0000
#define ISP_USRCTRL_CONTRAST    0x0001

#include "mv_types.h"
#include "sendOutApi.h"

void OnUserControlEvent(uint32_t ctrlId, uint16_t value);
void getUserControlsValues(uint16_t * pu16Exposure, uint16_t  * pu16Gain);
void getUserControlsActions(uint32_t * pu32Action);

#endif /* ISPUSERCONTROLSDATA_H_ */
