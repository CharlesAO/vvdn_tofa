///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Camera wrapper.
///
/// This implements silicon specific functions of CamGeneric
///
///

// 1: Includes
// ----------------------------------------------------------------------------
// Drivers includes

#include "assert.h"
#include "string.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "DrvIcb.h"
#include "DrvMipi.h"
#include "DrvCif.h"

// Level for camera interrupts
#define MIPI_INTERRUPT_LEVEL  (2)

// Dynamic IRQ's used
#define ROUTED_IRQ_MIPI       IRQ_DYNAMIC_5 // MIPI interrupt

//-------------------------------------------

// Fix for the Cif Bug 22697
#define D_CIF_CTRL_WINDOW_DISAB_MASK    (uint32_t)~(D_CIF_CTRL_WINDOW_EN)
#define D_MIPI_IRQ_STAT_HS_RX_BITS_POS  (14)
#define D_MIPI_IRQ_STAT_HS_RX_BITS_MASK (0x3f)
#define D_MIPI_CSI_FRAME_END            (0x01)

//---------------------------------------------------------------
// Fix for the Cif Bug 22697
static void mipiIrqHandler(uint32_t unused)
{
    uint32_t intStatus = 0;
    uint32_t MipiCtrlCifCfg = 0;
    uint32_t cifCtrl = 0;

    // Get/clear the status
    intStatus = GET_REG_WORD_VAL(MIPI_IRQ_ENABLE_ADDR);
    SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, intStatus);

    intStatus >>= D_MIPI_IRQ_STAT_HS_RX_BITS_POS;  // Obtain the interrupting controller

    // Get MIPI Rx controller -> CIF connection configuration
    // This is a one-hot bit vector that can be used to mask the interrupt
    // status, LSBs are for CIF0, MSBs for CIF1
    MipiCtrlCifCfg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);

    if (intStatus & MipiCtrlCifCfg & D_MIPI_IRQ_STAT_HS_RX_BITS_MASK)  // Validate the ISR against the enabled controllers & also in the range 0-7 (??)
    {
        // Disable and re-enable CIF0 windowing
        cifCtrl = GET_REG_WORD_VAL(CIF1_CONTROL_ADR);
        SET_REG_WORD(CIF1_CONTROL_ADR, cifCtrl & D_CIF_CTRL_WINDOW_DISAB_MASK);
        SET_REG_WORD(CIF1_CONTROL_ADR, cifCtrl);
    }

    MipiCtrlCifCfg >>= DRV_MIPI_NO_CTRL;

    if (intStatus & MipiCtrlCifCfg & D_MIPI_IRQ_STAT_HS_RX_BITS_MASK) {
        // Disable and re-enable CIF1 windowing
        cifCtrl = GET_REG_WORD_VAL(CIF2_CONTROL_ADR);
        SET_REG_WORD(CIF2_CONTROL_ADR, cifCtrl & D_CIF_CTRL_WINDOW_DISAB_MASK);
        SET_REG_WORD(CIF2_CONTROL_ADR, cifCtrl);
    }
}

static uint32_t getMipiBase(uint32_t mipiCtrlNo)
{
    switch (mipiCtrlNo) {
        case DRV_MIPI_C0:
            return (APB_MIPI0_HS_BASE_ADDR);
            break;
        case DRV_MIPI_C1:
            return (APB_MIPI1_HS_BASE_ADDR);
            break;
        case DRV_MIPI_C3:
            return (APB_MIPI3_HS_BASE_ADDR);
            break;
        case DRV_MIPI_C5:
            return (APB_MIPI5_HS_BASE_ADDR);
            break;
        default :
            assert(0);
    }
}

static void setupMipiIrq(uint32_t mipiCtrlNo, uint32_t reRouteIsr)
{
    uint32_t mipiBaseReg = 0;

    SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, (D_MIPI_IRQ_STAT_HS_RX_EVENT_0 << mipiCtrlNo));

    mipiBaseReg = getMipiBase(mipiCtrlNo);

    // Setup Mipi to generate IRQ on frame end packet
    SET_REG_WORD(mipiBaseReg + MIPI_RX_HS_PH_FILTER0_OFFSET, (1 << D_MIPI_CSI_FRAME_END) );
    SET_REG_WORD(mipiBaseReg + MIPI_RX_HS_PH_FILTER1_OFFSET, 0);
    SET_REG_WORD(MIPI_IRQ_ENABLE_ADDR,  GET_REG_WORD_VAL(MIPI_IRQ_ENABLE_ADDR) | (D_MIPI_IRQ_STAT_HS_RX_EVENT_0 << mipiCtrlNo));

    DrvIcbDynamicIrqConfig(LRT_TO_LOS, IRQ_MIPI, ROUTED_IRQ_MIPI, reRouteIsr);
    DrvIcbSetupIrq(IRQ_MIPI, MIPI_INTERRUPT_LEVEL, POS_EDGE_INT, (irq_handler)&mipiIrqHandler);
}

static void stopMipiIrq(uint32_t mipiCtrlNo)
{
    uint32_t mipiBaseReg = 0;

    mipiBaseReg = getMipiBase(mipiCtrlNo);

    // Stop Mipi to generate IRQ on frame end packet
    SET_REG_WORD(mipiBaseReg + MIPI_RX_HS_PH_FILTER0_OFFSET, 0);
    SET_REG_WORD(mipiBaseReg + MIPI_RX_HS_PH_FILTER1_OFFSET, 0);
    SET_REG_WORD(MIPI_IRQ_ENABLE_ADDR,  GET_REG_WORD_VAL(MIPI_IRQ_ENABLE_ADDR) & ~(D_MIPI_IRQ_STAT_HS_RX_EVENT_0 << mipiCtrlNo));

    // Disable ISR generation in ICB only if there is no other MIPI generating ISRs
    if ((GET_REG_WORD_VAL(MIPI_IRQ_ENABLE_ADDR) == 0 ))
    {
        DrvIcbDisableIrq(IRQ_MIPI);
        DrvIcbDynamicIrqConfig(LRT_TO_LOS, IRQ_MIPI, ROUTED_IRQ_MIPI, 0);
    }
}

void siliconSpecificIsrExtraSetup(uint32_t nbOflanes, uint32_t mipiCtrlNo, uint32_t reRouteIsr)
{
    // Fix for the Cif Bug 22697 - only needed for MA2150_A0
    if (nbOflanes > 0) {
        setupMipiIrq(mipiCtrlNo, reRouteIsr);
    }
    else {
        //TODO: implement GPIO VSYNC edge workaround for parallel sensors (equivalent for MIPI workaround, see 22697)
    }
}

void siliconSpecificIsrStop(uint32_t nbOflanes, uint32_t mipiCtrlNo)
{
    //fix for the Cif Bug 22697 - only needed for MA2150_A0
    if (nbOflanes > 0) {
        stopMipiIrq(mipiCtrlNo);
    }
    else {
        //TODO: stop the equivalent GPIO ISR generation for parallel sensors
    }
}
