///
/// @file OsDrvUsbPhyArch.c
/// 
/// 
/// @ingroup OsDrvUsbPhyArch
/// @{
///

#include <OsDrvUsbPhyArch.h>
#include <OsDrvUsbPhyDefines.h>
#include <registersMyriad.h>
#include <DrvRegUtils.h>

void OsDrvUsbPhyInitBusSettings(void)
{
    // Setup USB_GSBUSCFG0, AHB definitions, databook page 562
    // [0] INCRBRSTENA   = 0x1 = incremental burst enable (if no other bits set, burst length is 1)
    // [1] INCR4BRSTENA  = 0x? = if bit0=0x1 then burst length upto 4
    // [2] INCR8BRSTENA  = 0x? = if bit0=0x1 then burst length upto 8
    // [3] INCR16BRSTENA = 0x? = if bit0=0x1 then burst length upto 16

    SET_REG_WORD(
            USB_GSBUSCFG0,
            USB_GSBUSCFG0_INCR16BRSTENA |
            USB_GSBUSCFG0_INCR8BRSTENA |
            USB_GSBUSCFG0_INCR4BRSTENA
    );
}

///@}
