///
/// @file DrvSipp.h
/// 
/// 
/// @date Jul 22, 2014
/// @defgroup DrvSipp Sipp Driver
/// @{
/// @brief Sipp Driver configuration and functionality.
///


#ifndef DRVSIPP_H_
#define DRVSIPP_H_

#include "DrvSippDefines.h"
#include <registersMyriad.h>
#include <DrvRegUtils.h>


//======================================================================================
//==========  THIS IS A DRIVER FOR SIPP MIPI ONLY, NOT FOR GENERAL SIPP USAGE! =========
//======================================================================================


#ifdef __cplusplus
extern "C" {
#endif

///Initialize specific SIPP filters
///@param device The bitmask for initialized filters
///The bit numbers are corresponding to SIPP filter IDs which can be found in SIPP documentation(Table 21)
void DrvSippInit(u32 device);

///Reset specific SIPP filters
///@param device The bitmask for filters to reset
///The bit numbers are corresponding to SIPP filter IDs which can be found in SIPP documentation(Table 21)
void DrvSippReset(u32 device);

///filter restart
static inline void DrvSippEngineRestart( u8* bufPtr, u32 baseAddr, u32 filterNr)
{
    u32 sippBit = 1 << (filterNr + SIPP_MIPI_RX0_ID);
    sipp_buf_t *currentSippBuf = (sipp_buf_t *)baseAddr;

    SET_REG_WORD( SIPP_CONTROL_ADR, GET_REG_WORD_VAL(SIPP_CONTROL_ADR) & (~sippBit) );
    currentSippBuf->base = (u32)bufPtr;
    SET_REG_WORD( SIPP_CONTROL_ADR, GET_REG_WORD_VAL(SIPP_CONTROL_ADR) | sippBit );
}

///update the filter address in fill control mode
///usually, the buffer fill control mode does not request any address update, as it automatically manage a static allocated circular buffer with some lines.
///anyway, the address update might be needed in case distinct (separated) line buffers are needed by the application, possible in a dynamic allocation mode
static inline void DrvSippAddrUpdate( u8* bufPtr, u32 baseAddr, u32 filterNr)
{
    UNUSED(filterNr); // hush the compiler warning.

    sipp_buf_t *currentSippBuf = (sipp_buf_t *)baseAddr;

    currentSippBuf->base = (u32)bufPtr;
}

///Enable specific SIPP Rx filter
///@ param filterNr The SIPP Rx filter number. There are two available options:
/// - 0 - for SIPP Rx0
/// - 1 - for SIPP Rx1
void DrvSippEnableRxFilter(u32 filterNr);
void DrvSippDisableRxFilter(u32 filterNr);


///Enable specific SIPP Tx filter
///@ param filterNr The SIPP Tx filter number. There are two available options:
/// - 0 - for SIPP Tx0
/// - 1 - for SIPP Tx1

void DrvSippEnableTxFilter(u32 filterNr);

///Enable End of frame interrupt for specific SIPP Rx filter
///@ param filterNr The SIPP Rx filter number. There are two available options:
/// - 0 - for SIPP Rx0
/// - 1 - for SIPP Rx1
void DrvSippEnableRxFilterInterrupt2(u32 filterNr);

///Enable End of frame interrupt for specific SIPP Tx filter
///@ param filterNr The SIPP Tx filter number. There are two available options:
/// - 0 - for SIPP Tx0
/// - 1 - for SIPP Tx1
void DrvSippEnableTxFilterInterrupt2(u32 filterNr);

///End of frame interrupt configuration for SIPP Tx filters
///Align the end of frame interrupt with
///the input buffer fill level decrement interrupt.
///By default it is aligned with the output buffer fill level increment interrupt,
///unless the filter has no output buffer, e.g. MIPI Tx filters.
///@ param filterNr The SIPP Tx filter number. There are two available options:
/// - 0 - for SIPP Tx0
/// - 1 - for SIPP Tx1
void DrvSippCfgTxFilterInterrupt2(u32 filterNr);

///Output buffer configuration for SIPP Rx buffers
///@param filterNr The SIPP Rx filter number. There are two available options:
/// - 0 - for SIPP Rx0
/// - 1 - for SIPP Rx1
///@param settings #sipp_buf_t structure mapped over the output buffer configuration registers which can be used to directly configure them.
///A detailed description on output buffer configuration can be found in SIPP documentation(chapter 1.1.9)
void DrvSippConfigureMipiRxBuffers(u32 filterNr, sipp_buf_t* settings);

///Input buffer configuration for SIPP Tx buffers
///@param filterNr The SIPP Tx filter number. There are two available options:
/// - 0 - for SIPP Tx0
/// - 1 - for SIPP Tx1
///@param settings #sipp_buf_t structure mapped over the input buffer configuration registers which can be used to directly configure them.
///A detailed description on input buffer configuration can be found in SIPP documentation(chapter 1.1.8)
void DrvSippConfigureMipiTxBuffers(u32 filterNr, sipp_buf_t* settings);

///Configure SIPP Rx filter settings
///@param filterNr The SIPP Rx filter number. There are two available options:
/// - 0 - for SIPP Rx0
/// - 1 - for SIPP Rx1
///@param settings #sipp_rx_t structure mapped over the configuration registers.
///A detailed description of MIPI Rx fileter registers can be found in SIPP documentation(chapter 1.1.10)
void DrvSippConfigureMipiRx(u32 filterNr, sipp_rx_t* settings);

///Configure SIPP Rx filter settings
///@param filterNr The SIPP Tx filter number. There are two available options:
/// - 0 - for SIPP Tx0
/// - 1 - for SIPP Tx1
///@param settings #sipp_tx_t structure mapped over the configuration registers.
///A detailed description of MIPI Rx fileter registers can be found in SIPP documentation(chapter 1.1.10)
void DrvSippConfigureMipiTx(u32 filterNr, sipp_tx_t* settings);

///Start the output of data from the specific Tx filter
///@param filterNr The SIPP Tx filter number. There are two available options:
/// - 0 - for SIPP Tx0
/// - 1 - for SIPP Tx1
void DrvSippStartTx(u32 filterNr);

inline void DrvSippEnableClk(void)
{
    SET_REG_WORD(MSS_SIPP_CLK_CTRL_ADR, 0xffffffff);
}

#ifdef __cplusplus
}
#endif

#endif /* DRVSIPP_H_ */

///@}

