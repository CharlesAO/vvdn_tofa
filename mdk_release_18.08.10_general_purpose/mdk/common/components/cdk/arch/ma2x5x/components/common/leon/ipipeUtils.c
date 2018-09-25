#include <mv_types.h>
#include <assert.h>
#include "VcsHooksApi.h"
#include "UnitTestApi.h"
#include "ipipe.h"

//#############################################################################
//cross platform: PC, Myriad
uint16_t f32_to_f16(float f)
{
    uint32_t in;
    int	     expo;
    uint32_t *p32 = (uint32_t *)&f;

    in = *p32;
    expo = (in & 0x7f800000) >> 23;
    expo = expo - 127 + 15;
    if (expo > 31) {
        expo = 31;
    }
    if (expo < 0) {
        expo = 0;
    }
    return (in & (1<<31)) >> 16 | expo << 10 | (in & 0x007fffff) >> 13;
}

//#############################################################################
//cross platform: PC, Myriad
uint16_t int_to_f16(int i)
{
    return f32_to_f16(i);
}

//#############################################################################
#if defined(__sparc)
void dumpAndVerification(char *refName, void *refBuf, void *outputBuf, unsigned int size)
{
    // Dump the outputs, and verify agains expected output
    dumpMemoryToFile((unsigned int)outputBuf, size);
    loadMemFromFileSimple(refName, size, refBuf);
    unitTestMemCompare((void*)refBuf, (void*)outputBuf, size);
}
#endif

//#############################################################################
//Returns line width in BYES considering bits per pixel and available MA2150 packing modes
uint32_t lineWidthInBytes(uint32_t widthInPixels, uint32_t bpp)
{
    uint32_t w = widthInPixels;
    switch(bpp)
    {
       case 10: w = w * 5 / 4; break; //10bit pack
       case 12: w = w * 3 / 2; break; //12bit pack
       case 16: w = w * 2;     break; //flat 16bit
       case  8:                break; //flat 8bit
       default: assert(0);     break; //invalid, (e.g.for 14bit must specify 16bit storage)
    }
    return w;
}

#ifdef DBG_DUMP_GRAPH
//__attribute__((section(".ddr.data")))  ALIGNED
uint8_t dbgGr[7563];
#include <string.h>
//dot -T pdf -o pipeIsp.pdf < pipeIsp.dot
void dumpDotFileLrtAppGraph(icCtrl *ctrl) {
    uint32_t        idx = 0;
    uint32_t        tt = 0;
    IcQueryPlg      *plgDescription;
    sprintf(&dbgGr[strlen(dbgGr)], "//dot -T pdf -o pipeIsp.pdf < pipeIsp.dot \n");
    sprintf(&dbgGr[strlen(dbgGr)], "digraph G {\n");
    for (tt = IC_QUERY_TYPE_SOURCE; tt < IC_QUERY_TYPE_MAX; tt++) {
        idx = 0;
        while ((plgDescription = (IcQueryPlg *)icQuery(ctrl, tt, idx)) != NULL) {
            uint32_t x = 0;
            for (x = 0; x < plgDescription->plgLink.nrChilds; x++) {
                sprintf(&dbgGr[strlen(dbgGr)], "\t%s_%d -> %s_%d;\n",
                        plgDescription->plgDefine.name, plgDescription->plgDefine.idx,
                        plgDescription->plgLink.child[x]->name, plgDescription->plgLink.child[x]->idx);
            }
            idx++;
        }
    }
    sprintf(&dbgGr[strlen(dbgGr)], "\tLegend [ label = \"LEGEND\\n PuginName_Idx \" ];\n");
    sprintf(&dbgGr[strlen(dbgGr)], "}\n");
    saveMemoryToFile((uint32_t)dbgGr, sizeof(dbgGr), "pipeIsp.dot");
}
#endif
