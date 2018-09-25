///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// -----------------------------------------------------------------------------
///
/// Revision History
/// ===================================
/// 05-Nov-2014 : Author ( MM Solutions AD )
/// Created
/// =============================================================================

#ifndef _MIPI_TX_BOARD_PARAMS_H
#define _MIPI_TX_BOARD_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <DrvMipiDefines.h>
#include <mv_types.h>

#define BRD_REF_OSC_CLOCK           12000

#define BRD_MIPI_TX_MIPI_CTRL_NO    MIPI_CTRL_5
#define BRD_MIPI_TX_NUM_LANES       2
#define BRD_MIPI_TX_CLOCK           400
#define BRD_MIPI_TX_USE_IRQ         1

typedef struct {
   eDrvMipiCtrlNo mipi_controller;
   u32 mipi_ref_clock_kHz;
} mipi_tx_board_params_t;

extern mipi_tx_board_params_t *mipi_tx_board_params;

#ifdef __cplusplus
}
#endif

#endif /* _MIPI_TX_BOARD_PARAMS_H */

