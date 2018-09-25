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
* @file sg_ae_distribution.h
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

#ifndef  __SG_AE_DISTRIBUTION_H__
#define  __SG_AE_DISTRIBUTION_H__

#include "sg_ae_distribution_types.h"

typedef void* sg_distr_ae_hndl_t;

int sg_distr_ae_create(sg_distr_ae_hndl_t *sg_distr_ae_hndl);
int sg_distr_ae_destroy(sg_distr_ae_hndl_t sg_distr_ae_hndl);
int sg_distr_ae_config(sg_distr_ae_hndl_t sg_distr_ae_hndl, sg_ae_distr_config_t* cfg);
int sg_distr_ae_process(sg_distr_ae_hndl_t sg_distr_ae_hndl, sg_ae_distr_calc_input_t* distr_inp,
                           ae_distr_calc_output_t *out);


#endif // __SG_AE_DISTRIBUTION_H__

