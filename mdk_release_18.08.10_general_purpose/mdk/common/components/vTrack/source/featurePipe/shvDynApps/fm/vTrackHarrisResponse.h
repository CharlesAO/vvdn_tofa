#ifndef _HARRIS_RESPONSE_H
#define _HARRIS_RESPONSE_H
#include <mv_types.h>
#ifdef __cplusplus
extern "C"
{
#endif
fp32 harrisResponse(u8 *data, u32 x, u32 y, u32 step_width, fp32 k);
#ifdef __cplusplus
}
#endif
#endif
