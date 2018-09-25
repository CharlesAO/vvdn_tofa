/* =============================================================================
* Copyright (c) 2013-2014 MM Solutions AD
* All rights reserved. Property of MM Solutions AD.
*
* This source code may not be used against the terms and conditions stipulated
* in the licensing agreement under which it has been supplied, or without the
* written permission of MM Solutions. Rights to use, copy, modify, and
* distribute or disclose this source code and its documentation are granted only
* through signed licensing agreement, provided that this copyright notice
* appears in all copies, modifications, and distributions and subject to the
* following conditions:
* THIS SOURCE CODE AND ACCOMPANYING DOCUMENTATION, IS PROVIDED AS IS, WITHOUT
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED. MM SOLUTIONS SPECIFICALLY DISCLAIMS
* ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN
* NO EVENT SHALL MM SOLUTIONS BE LIABLE TO ANY PARTY FOR ANY CLAIM, DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
* PROFITS, OR OTHER LIABILITY, ARISING OUT OF THE USE OF OR IN CONNECTION WITH
* THIS SOURCE CODE AND ITS DOCUMENTATION.
* =========================================================================== */
/**
* @file aca_hllc_api.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 05-Nov-2013 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef  __HLLC_API_3_H__
#define  __HLLC_API_3_H__

#include "osal/osal_stdtypes.h"
#include "aca_hllc_types.h"

/* =============================================================================
 *                                   CODE SECTION
 * =========================================================================== */

/* ========================================================================== */
/**
* @func hllc_init()
*
* @brief Load HLLC algorithm instance. Initialize hllc contex.
*
* @param [in/out] context - void ** - Input and output parameters of the hllc.
*
* @param [in] dtp_data - void * - DTP data.
*
* @return hllc_status_t - status of the operations.
*
*/
/* ========================================================================== */

hllc_status_t hllc_init(void **context, void *dtp_data);

/* ========================================================================== */
/**
* @func hllc_reinit()
*
* @brief Reinitialize hllc contex.
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @param [in] dtp_data - void * - Pointer to DTP data.
*
* @return hllc_status_t - status of the operations.
*
*/
/* ========================================================================== */

hllc_status_t hllc_reinit(void *context, void *dtp_data);

/* ========================================================================== */
/**
* @func hllc_config()
*
* @brief Configure context with dtp end eeprom data. Update tuning data.
* Configure lens to move to initial position.
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @param [in] hllc_cfg - hllc_cfg_t - Configure data.

*
* @return hllc_status_t - status of the operations.
*
*/
/* ========================================================================== */

hllc_status_t hllc_config(void *context, hllc_cfg_t *hllc_cfg);

/* ========================================================================== */
/**
* @func hllc_del()
*
* @brief Delete HLLC algorithm instance.
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @return hllc_status_t - status of the operations
*
*/
/* ========================================================================== */

hllc_status_t hllc_del(void *context);

/* ========================================================================== */
/**
* @func hllc_mov_calc()
*
* @brief Calculations for lens move executions.
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @param [in] in_data - aca_hllc_calc_input_t * - Move command.
*
* @param [out] out_data - aca_hllc_calc_output_t * - Output steps and timing parameters
*
* @return hllc_status_t - status of the operations
*
*/
/* ========================================================================== */

hllc_status_t hllc_mov_calc(void *context, aca_hllc_calc_input_t *in_data, aca_hllc_calc_output_t * out_data);

/* ========================================================================== */
/**
* @func hllc_mov2pos_calc()
*
* @brief Calculations for move to position lens executions..
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @param [in] in_data - aca_hllc_calc_input_t - Input data containing current lens position
*
* @param [out] out_data - aca_hllc_calc_output_t * - Output list ot steps and delays.
*
* @return hllc_status_t - status of the operations
*
*/
/* ========================================================================== */

hllc_status_t hllc_mov2pos_calc(void *context, aca_hllc_calc_input_t *in_data, aca_hllc_calc_output_t * out_data);

/* ========================================================================== */
/**
* @func hllc_calc_next_step()
*
* @brief Calculate times and lens movement before lens move.
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @param [out] lens_move - lens_drv_move_t * - Lens movement command.
*
* @return hllc_status_t - status of the operations
*
*/
/* ========================================================================== */

hllc_status_t hllc_calc_next_step(void *context, lens_drv_move_t *lens_move);

/* ========================================================================== */
/**
* @func hllc_lens_abort()
*
* @brief Abort moving the lens.
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @return hllc_status_t - status of the operations
*
*/
/* ========================================================================== */

hllc_status_t hllc_lens_abort(void *context);

/* ========================================================================== */
/**
* @func hllc_mov2pos_query_total_time()
*
* @brief Query total time for move to position command.
*
* @param [in/out] context - void * - Input and output parameters of the hllc.
*
* @param [in] query - hllc_query_absolute_t * - Move to position query.
*
* @param [out] total_time - int * - Total calculated time for lens movement.
*
* @return hllc_status_t - status of the operations
*
*/
/* ========================================================================== */

hllc_status_t hllc_mov2pos_query_total_time(void *context,
                                            hllc_query_absolute_t *query,
                                            int *total_time);

/* ========================================================================== */
/**
* @func hllc_fill_dtp_content()
*
* @brief Fill the DTP content.
*
* @param    context - void * - HLLC algorithm context.
* @param    dtp - void * - DTP driver handle.
*
* @return hllc_status_t - status of the operations
*/
/* ========================================================================== */
hllc_status_t hllc_fill_dtp_content(void *context, void *dtp);


#endif // __HLLC_API_H_3__
