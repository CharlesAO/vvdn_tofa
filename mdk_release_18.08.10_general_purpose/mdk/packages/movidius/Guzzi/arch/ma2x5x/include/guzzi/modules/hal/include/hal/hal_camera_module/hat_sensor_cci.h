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
* @file hat_sensor_cci.h
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

#ifndef __HAT_TYPES_CCI_H__
#define __HAT_TYPES_CCI_H__

#include <hal/hat_types.h>

// ----------- CCI Device manipulation  -------------------------



/**
 * Generic opaque driver handle representing device
 * It is returned by driver on hat_bus_cci_dev_create
 *
 * @see hat_bus_cci_dev_create
 */
typedef void* hat_cci_dev_hndl_t;

/**
 * Generic opaque handle - client provides for each CCI device
 * The driver will return it back to CCI client with callback
 * when transaction finish.
 *
 * @see hat_dev_cci_open
 */
typedef void* hat_cci_client_hndl_t;

/**
 * Generic opaque handle - client provides for each CCI transaction
 * The driver will return it back to CCI client with callback
 * when transaction finish.
 *
 * @see hat_dev_cci_write
 * @see hat_dev_cci_read
 * @see hat_dev_callback
 */
typedef void* hat_cci_transaction_hndl_t;

/**
 * Device callback - invoked at the end of transaction (read or write)
 * or on error
 *
 * @param h Device handle
 * @param *status transaction status. 0 for success
 * @param client_prv client private data
 * @param trans_prv  transaction private data
 */
typedef void (*hat_dev_callback_t)(hat_cci_transaction_hndl_t trans_prv, uint32 *status);

/**
 * CCI Device create parameters
 *
 * @param dev_identity    Device device identity Camera instance + Sensor instance (primary, secondary etc)
 * @param dev_address     device CCI address
 * dev_sub_address;       device sub-address - not relevant for now
 * @param dev_address_len CCI address length in Bits
 */
typedef struct {
    uint32  dev_identity;       // device device identity
    uint32  dev_address;        // device address
    uint32  dev_sub_address;    // device sub-address - not relevant for now
    uint32  dev_address_len;    // [Bits]
    hat_dev_callback_t cb;
} hat_cci_dev_create_param_t;


/**
 * CCI object
 *
 * @param client_handle    Implementation specific handle
 * @param write            pointer to CCI implementation
 * @param read             pointer to CCI implementation
 *
 * @see hat_dev_cci_dev_create
 */
typedef struct {
   hat_cci_client_hndl_t client_handle;
   hat_cci_write_t write;
   hat_cci_read_t  read;
} hat_cci_object_t;

/**
 * Handle to CCI object
 *
 * @see hat_cci_object_t
 * @see hat_dev_cci_dev_create
 */
typedef hat_cci_object_t* hat_cci_hndl_t;

/**
 * Pointer to Platform specific API.
 * Writes num bytes pointed by data on CCI Device
 *
 * @param h Client handle
 * @param reg_addr register start address where data should be written
 * @param num Number of bytes that should be written
 * @param data Pointer to data that should be written
 * @param trans_prv Client private data specific to this transaction
 * @return 0 on success
 */
typedef int (*hat_cci_write_t) (hat_cci_client_hndl_t h, uint32 reg_addr, uint32 num, const void* data, hat_cci_transaction_hndl_t trans_prv);

/**
 * Pointer to Platform specific API.
 * Reads num bytes from CCI Device.
 *
 * @param h         Client handle
 * @param reg_addr  register start address to read data from
 * @param num       Number of bytes to read
 * @param data      Pointer to store data
 * @param trans_prv Client private data specific to this transaction
 * @return 0 on success
 */
typedef int (*hat_cci_read_t) (hat_cci_client_hndl_t h, uint32 reg_addr, uint32 num, void* data, hat_cci_transaction_hndl_t trans_prv);


/**
 * Creates a new device on CCI Bus
 * Implementation is Platform specific
 *
 * @param bh     Bus handle
 * @param h[out] Device handle
 * @param param  Create parameters
 * @return 0 on success
 */
int hat_dev_cci_dev_create (hat_cci_hndl_t* h, hat_cci_dev_create_param_t params);

/**
 * Destroy CCI device.
 * Implementation is Platform specific.
 *
 * @param h Device handle
 * @return 0 on success
 */
int hat_dev_cci_dev_desroy (hat_cci_hndl_t h);


/**
 * Writes num bytes pointed by data on CCI Device
 *
 * @param h Client handle
 * @param reg_addr register start address where data should be written
 * @param num Number of bytes that should be written
 * @param data Pointer to data that should be written
 * @param trans_prv Client private data specific to this transaction
 * @return 0 on success
 */
static inline int hat_cci_dev_write (hat_cci_hndl_t h, uint32 reg_addr, uint32 num, const void* data, hat_cci_transaction_hndl_t trans_prv)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->write))
    {
        return 1;
    }
    ret_val = h->write(h->client_handle, reg_addr, num, data, trans_prv);

    return ret_val;
}

/**
 * Reads num bytes from CCI Device
 *
 * @param h         Client handle
 * @param reg_addr  register start address to read data from
 * @param num       Number of bytes to read
 * @param data      Pointer to store data
 * @param trans_prv Client private data specific to this transaction
 * @return 0 on success
 */

static inline int hat_cci_dev_read (hat_cci_hndl_t h, uint32 reg_addr, uint32 num, const void* data, hat_cci_transaction_hndl_t trans_prv)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->read))
    {
        return 1;
    }
    ret_val = h->read(h->client_handle, reg_addr, num, data, trans_prv);

    return ret_val;
}

#endif // __HAT_TYPES_CCI_H__
