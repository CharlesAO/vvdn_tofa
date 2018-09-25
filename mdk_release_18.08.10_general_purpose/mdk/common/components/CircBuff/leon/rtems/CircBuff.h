#ifndef __CIRC_BUFF_H__
#define __CIRC_BUFF_H__

#include <stdint.h>
#include <OsDrvCmxDma.h>

typedef enum // Transfer direction
{
    Cmx2Ddr = 0, // circular to full (full buffer can be in CMX as well...)
    Ddr2Cmx = 1, // full to circular
}TransferDir;

//=====================================================================
typedef struct
{
    uint32_t base;
    uint32_t lnStride;
    uint32_t plStride;
    uint32_t curLine;
    uint32_t height;
} Buff;

//=====================================================================
typedef struct
{
    uint32_t runNo;
    uint32_t lnPerRun;
    uint32_t width;   // [line width]
    uint32_t cpLines; // should match ddr.height eventually
    Buff     cmx;
    Buff     ddr;

    TransferDir          dir;
    // TBD: use multiple pending transactions, fetch one as in Opipe
    OsDrvCmxDmaTransactionHnd h;
    OsDrvCmxDmaTransaction    t;

  #if defined(CBUFF_PAD)
    uint32_t cmxPad;
  #endif
} CBuff;

//=====================================================================
// Init CDMA descriptor(s)
int32_t CircBuffInit(CBuff *cb, TransferDir dir,
                  uint32_t cmxAddr, uint32_t ddrAddr,
                  uint32_t cmxH   , uint32_t ddrH,
                  uint32_t totLen , uint32_t lineW);


int32_t CircBuffFeed(CBuff *cb);
#endif //__CIRC_BUFF_H__
