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
* @file aca_awb.h
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

#ifndef  __ACA_INTERFACE_AWB_H__
#define  __ACA_INTERFACE_AWB_H__

#include "osal/osal_stdtypes.h"
#include <aca_awb_types.h>

typedef void* aca_awb_hndl_t;

int aca_awb_create(aca_awb_hndl_t *aca_awb_hndl);

int aca_awb_destroy(aca_awb_hndl_t aca_awb_hndl);

int aca_awb_configuration(aca_awb_hndl_t aca_awb_hndl,
                             aca_awb_config_t* cfg);

int aca_awb_process(aca_awb_hndl_t aca_awb_hndl,
                        aca_awb_calc_input_t* awb_inp,
                        awb_calc_output_t *out);

int awb_convert_gains2kelvins(aca_awb_hndl_t aca_awb_hndl,
    float gainR, float gainG,
    float gainB, uint32* kelvins);

int awb_convert_kelvins2gains(aca_awb_hndl_t aca_awb_hndl, uint16 kelvins,
    float *gainR, float *gainG, float *gainB);

#endif /* __ACA_INTERFACE_AWB_H__ */

