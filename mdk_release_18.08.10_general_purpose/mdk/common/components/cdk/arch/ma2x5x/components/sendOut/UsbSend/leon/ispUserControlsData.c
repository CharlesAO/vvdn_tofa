#include "ispUserControlsData.h"

static uint16_t u16Exposure;
static uint16_t u16Gain;

void OnUserControlEvent(uint32_t ctrlId, uint16_t u16Value)
{
	switch (ctrlId)
	{
		case ISP_USRCTRL_BRIGHTNESS: { u16Exposure = u16Value; break;}
		case ISP_USRCTRL_CONTRAST:   { u16Gain     = u16Value * 256; break;}
		default: {}
	}
}

void getUserControlsValues(uint16_t *pu16Exposure, uint16_t* pu16Gain)
{
	*pu16Exposure = u16Exposure;
	*pu16Gain     = u16Gain;
}
