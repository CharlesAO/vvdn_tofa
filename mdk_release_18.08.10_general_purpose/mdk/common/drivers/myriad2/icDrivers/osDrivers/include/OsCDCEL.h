#ifndef OS_CDCEL_H
#define OS_CDCEL_H

#include <rtems.h>
#include <rtems/libi2c.h>

#include <mv_types.h>

#define CDCEL_NAME "cdcel"

typedef enum cdcelFullConfiguration_t
{
    EXT_PLL_CFG_74MHZ        = 1,
    EXT_PLL_CFG_111MHZ       = 2,
    EXT_PLL_CFG_148MHZ       = 3,
    EXT_PLL_CFG_74_24_24MHZ  = 4,  //74.25 Mhz on Y1 (for HDMI usage), 24 Mhz on Y4 + Y5 (for cameras usage)
    EXT_PLL_CFG_148_24_24MHZ = 5,  //148.5 Mhz on Y1 (for HDMI usage), 24 Mhz on Y4 + Y5 (for cameras usage)
    EXT_PLL_CFG_74_24_16MHZ  = 6,  //74.25 Mhz on Y1 (for HDMI usage), 24 Mhz on Y4 and + 16 Mhz on Y5 (for ca
} cdcelFullConfiguration_t;
/// CDCEL slave addresses
#define CDCEL913_SLAVE_ADDR (0xCA >> 1)
#define CDCEL925_SLAVE_ADDR (0xC8 >> 1)

static const u8 CDCEL_SLAVE_ADDR[] = {CDCEL913_SLAVE_ADDR, CDCEL925_SLAVE_ADDR};

#ifdef __cplusplus
extern "C" {
#endif

    extern const rtems_libi2c_drv_t cdcelProtocolDrvTbl;

#ifdef __cplusplus
}
#endif

#endif
