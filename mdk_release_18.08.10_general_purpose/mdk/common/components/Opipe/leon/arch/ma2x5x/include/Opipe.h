/// @defgroup Opipe Opipe
/// @brief The Opipe components is an optimized ISP pipeline composed of a chain of SIPP hardware accelerator filters.
///
/// @file
/// @author    andrei.lupas@movidius.com
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - API
///
/// @ingroup  Opipe
/// @{
///

#ifndef __O_PIPE_CFG_H__
#define __O_PIPE_CFG_H__

#include "OpipeBlocks.h"
#include "OpipeDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

///Performs system initializations suitable for most tests.
///These include: clocks enabling, setting LeonL2 cache in WT mode, lowering Leon interrupt level,
///setup test module.
///
void     OpipeTestInit(); //general test


///Performs Opipe general initialization
///
void     OpipeReset();


///Performs individual pipeline initialization
///
///@param [in] p          reference to pipeline to be initialized
void     OpipeInit  (Opipe *p);


///Starts individual pipeline
///
///@param [in] p          reference to pipeline to be started
uint32_t OpipeStart (Opipe *p);


///Waits for completion of current pipeline (BLOCKING!)
///
///@param [in] p          reference to pipeline to be waited for
uint32_t OpipeWait  (Opipe *p);


///Computes value to be OR-ed into SIPP_OPIPE_CFG_ADR
///
///@param [in] p          reference to pipeline of interest
void     OpipeDetCfg(Opipe *p); //setup stage public to factor setup time

///Defines new CMX circular buffer
///
///@param [in] p          reference to pipeline of interest
///@param [in] unitID     corresponding SIPP filter ID
///@param [in] flags      buffer flags: [DMA driven] and direction(mandatory)
///@param [in] cmxBuff    circular buffer base address
///@param [in] lineW      line width in pixels
///@param [in] cmxBuffH   circular buffer height [lines]
///@param [in] bpp        format (1,2,4 BPP or packed formats; see SIPP_FMT_*)
///@return                reference to newly created buffer
DBuffer* OpipeCfgBuff(Opipe    *p,
                      uint32_t  unitID,
                      uint32_t  flags,
                      uint32_t  cmxBuff,
                      uint32_t  cmxBuffH,
                      uint32_t  bpp);

SwLink* OpipeSwLink(Opipe   *p,
                    uint32_t prodID,
                    uint32_t allConsMask,
                    uint32_t lastConsID);

///Set default timing values to MipiTx debug loopback structure.
///
///@param [out] mipiTxCfg reference to structure to be initialized
///@param [in]  txID      sipp tx ID: SIPP_MIPI_TX0_ID or SIPP_MIPI_TX1_ID
///@param [in]  iBuf      address of main full buffer to be sent over mipiTx
///@param [in]  bpp       bytes per pixel
///@param [in]  imgW      image width  [pixels]
///@param [in]  imgH      image height [pixels]
void defaultMipiTxLoopParams(oMipiTxLoopbackParam *mipiTxCfg, uint32_t txID, uint32_t iBuf, uint32_t bpp, uint32_t imgW, uint32_t imgH);

///Set the Mipi Tx->RX loopback mode as per argument struct
///
///@param [in] cfg        reference to param structure
void oCfgMipiTxLoopback      (oMipiTxLoopbackParam *cfg);

///Start MipiTx timing generator.
///
///@param [in] txID       sipp tx ID: SIPP_MIPI_TX0_ID or SIPP_MIPI_TX1_ID
void oStartMipiTxLoopback   (uint32_t txID);

///Wait till RAW stats are written to memory (test purpose mainly)
///
void OpipeWaitForRawStats();

///Wait NUMX100 groups of 100 nops (debug/test)
///
///@param [in] numx100  number of 100NOP times to wait
void OpipeDelay(uint32_t numx100);

///Set new resolution
///
///@param [in] p          reference to pipeline of interest
///@param [in] newW       new main input width
///@param [in] newH       new main input height
void OpipeSetRes(Opipe *p, uint32_t newW, uint32_t newH);

/// Callback to override Luma format to u8f (default fp16)
void OpipeForceU8fLuma(Opipe *p);

///Set SIPP image
///
///@param [in] p          reference to pipeline of interest
void DrvSetSliceDbyrLumaBuff(Opipe *p, u32 sWidth, u32 firstSlc);

inline uint32_t DrvSetSippClkCtrlRegister(uint32_t mask_value){
	mask_value = mask_value & 0xDFFFFFF;
	SET_REG_WORD(MSS_SIPP_CLK_CTRL_ADR, mask_value);
	return GET_REG_WORD_VAL(MSS_SIPP_CLK_CTRL_ADR);
}

#ifdef __cplusplus
} //extern "C"
#endif

/// @}
#endif
