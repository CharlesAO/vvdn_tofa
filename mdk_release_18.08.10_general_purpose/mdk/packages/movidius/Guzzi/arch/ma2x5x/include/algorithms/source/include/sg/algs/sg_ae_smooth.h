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
* @file sg_ae_smooth.h
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

#ifndef  __ACA_INTERFACE_AE_SMOOTH_H__
#define  __ACA_INTERFACE_AE_SMOOTH_H__

#include "osal/osal_stdtypes.h"
#include "sg_ae_smooth_types.h"

typedef void* sg_ae_smooth_hndl_t;

int sg_ae_smooth_create(sg_ae_smooth_hndl_t *sg_ae_smooth_hndl);

int sg_ae_smooth_destroy(sg_ae_smooth_hndl_t sg_ae_smooth_hndl);

int sg_ae_smooth_config(sg_ae_smooth_hndl_t sg_ae_smooth_hndl, sg_ae_smooth_config_t* cfg);

int sg_ae_smooth_process(sg_ae_smooth_hndl_t sg_ae_smooth_hndl,sg_ae_smooth_input_t* smooth_inp,
                      ae_smooth_output_t *smooth_out);

int sg_ae_smooth_reset(sg_ae_smooth_hndl_t sg_ae_smooth_hndl, uint32 mode);

#endif // __ACA_INTERFACE_AE_SMOOTH_H__

