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
* @file hat_metadata.h
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

#ifndef __HAT_TYPES_METADATA_H__
#define __HAT_TYPES_METADATA_H__

#include <hal/hat_types.h>


/**
 * @fn callback_metadata_sent_t
 *
 * @param userData - user specific data pointer - provided with
 *                   hat_metada_transfer API
 * @param metadata_buffer - provided with hat_metada_transfer API
 *
 * @return void
 *
 * @see hat_metada_transfer
 */
typedef void callback_metadata_sent_t(void *userData, void *metadata_buffer);

/**
 * @fn hat_metada_transfer
 *
 * @param camera_id - instance id
 * @param seq_no (frame/request) sequence number for internal synchronization
 * @param metadata_buffer - pointer to metadata buffer
 * @param metadata_size - metadata buffer size
 * @param cb - callback. Invoked after the metadata buffer is free (consumed/sent)
 * @param userData - to be used with callback invocation
 *
 * @return 0 on success
 *
 * @see callback_metadata_sent_t
 */
int hat_metada_transfer(int32 camera_id,
                        int32 seq_no,
                        void* metadata_buffer,
                        uint32 metadata_size,
                        callback_metadata_sent_t cb,
                        void* userData);


#endif // __HAT_TYPES_METADATA_H__
