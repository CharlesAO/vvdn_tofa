#include <stdio.h>
#include "Opipe.h"
#include "isp_params_wrap.h"

MipiRxCfg mipiCfg;

//#############################################################################
void appSetParamsRxPipe(Opipe *p)
{
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Global stuff
    p->rawBits      = ISPC_BAYER_BITS;
    p->bayerPattern = BAYER_PATTERN;

  //SIPP_MIPI_RX0_ID
    p->pMipiRxCfg[0]        = &mipiCfg;
    mipiCfg.cfg             = 1; //clock mipi RX/2 (needed in loopback?)
    mipiCfg.xStartWidth [0] = (p->width << 16) | 0x0000;
    mipiCfg.yStartHeight[0] = ((p->height - 2) << 16) | 0x0000;
    mipiCfg.sel01           =  0xF << 8;
    mipiCfg.mask        [0] = (1 << ISPC_BAYER_BITS) - 1;
}
