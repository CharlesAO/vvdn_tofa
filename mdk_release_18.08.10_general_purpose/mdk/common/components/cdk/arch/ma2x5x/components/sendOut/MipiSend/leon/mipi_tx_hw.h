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

#ifndef _MIPI_TX_HW_H
#define _MIPI_TX_HW_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mipi_tx_hw mipi_tx_hw_t;

#define LAYERS_MAX 4

typedef struct {
    unsigned int width;
    unsigned int line_size;
    unsigned int height[LAYERS_MAX];
    unsigned int data_type[LAYERS_MAX];
    void *p[LAYERS_MAX];
} mipi_tx_hw_frame_t;

typedef void mipi_tx_hw_sent_callback_t(
        mipi_tx_hw_t *mipi_tx_hw,
        void *client_prv
    );

void mipi_tx_hw_poll(mipi_tx_hw_t *mipi_tx_hw);

int mipi_tx_hw_send(
        mipi_tx_hw_t *mipi_tx_hw,
        void *client_prv,
        mipi_tx_hw_frame_t *frame
    );

int mipi_tx_hw_start(mipi_tx_hw_t *mipi_tx_hw);
void mipi_tx_hw_stop(mipi_tx_hw_t *mipi_tx_hw);

void mipi_tx_hw_destroy(mipi_tx_hw_t *mipi_tx_hw);
mipi_tx_hw_t *mipi_tx_hw_create(
        int num_lanes,
        int mipi_clock,
        int use_irq,
        mipi_tx_hw_sent_callback_t *sent_callback
    );

#ifdef __cplusplus
}
#endif

#endif /* _MIPI_TX_HW_H */

