
#include "mv_types.h"
#include "sipp.h"

u16 lutTable[256] __attribute__((section(".cmx.data"))) __attribute__((aligned(8)));  //lut table in CMX only

void configureLutFilterSiliconSpecific(LutParam* lutCfg)
{
    lutCfg->lutFormat  = 2;
}
