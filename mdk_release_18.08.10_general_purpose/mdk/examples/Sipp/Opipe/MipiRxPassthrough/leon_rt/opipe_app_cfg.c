///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Opipe configuration file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
static MipiRxCfg mipiCfg;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void appSetParams(Opipe *p)
{
    //Global stuff
    p->rawBits      = ISPC_BAYER_BITS;
    p->bayerPattern = BAYER_PATTERN;

    //SIPP_MIPI_RX1_ID
    p->pMipiRxCfg[1]       = &mipiCfg;
    mipiCfg.cfg             = 1; //clock mipi RX/2 (needed in loopback?)
    mipiCfg.xStartWidth [0] = (p->width <<16) | 0x0000;
    mipiCfg.yStartHeight[0] = (p->height<<16) | 0x0000;
    mipiCfg.sel01           =  0xF<<8;
    mipiCfg.mask        [0] = (1<<ISPC_BAYER_BITS)-1;
}
