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
* @file aca_hllc.h
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

#ifndef  __ACA_INTERFACE_HLLC_H__
#define  __ACA_INTERFACE_HLLC_H__

#include "osal/osal_stdtypes.h"
#include <aca_hllc_types.h>

typedef void* aca_hllc_hndl_t;

int aca_hllc_create(aca_hllc_hndl_t *aca_hllc_hndl);

int aca_hllc_destroy(aca_hllc_hndl_t aca_hllc_hndl);

int aca_hllc_config(aca_hllc_hndl_t aca_hllc_hndl,
                             aca_hllc_config_t* cfg);

int aca_hllc_process(aca_hllc_hndl_t aca_hllc_hndl,
                        aca_hllc_calc_input_t* hllc_inp,
                        aca_hllc_calc_output_t *out);

int aca_hllc_af_init_move(aca_hllc_hndl_t aca_hllc_hndl,
                      aca_hllc_calc_output_t *out);

#endif /* __ACA_INTERFACE_HLLC_H__ */

