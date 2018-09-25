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
* @file osal_mailbox.h
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

#ifndef  __OSAL_MAILBOX_H__
#define  __OSAL_MAILBOX_H__

#include <osal/osal_stdtypes.h>

struct osal_mailbox;

union osal_mbox_cmd {
    uint16 all;
    struct {
        uint16 id : 10;
        uint16 status : 5;
        uint16 ack : 1;
    };
};

/* System initialization of MAILBOX library */
int osal_mailbox_init(void);

/* System deinitialization of MAILBOX library */
int osal_mailbox_exit(void);

/* msg_size are use only when message has constant size, otherwise set 0 */
struct osal_mailbox *osal_mailbox_create(const char *name, uint32 msg_size);

int osal_mailbox_destroy(struct osal_mailbox *ctx);

int osal_mailbox_write(struct osal_mailbox *ctx,
        union osal_mbox_cmd *cmd, void *payload, int size);

/* timeout:
 *    -1  - blocked mode
 *    >=0 - with timeout in ms
 *
 * return value:
 *    -2   - Some errors
 *    -1   - Timeout
 *    >= 0 - Real size of payload size
 */
int osal_mailbox_readtimed(struct osal_mailbox *ctx,
        union osal_mbox_cmd *cmd, void *payload, int max_size, int timeout);

int osal_mailbox_read(struct osal_mailbox *ctx,
        union osal_mbox_cmd *cmd, void *payload, int max_size);

#endif /* __OSAL_MAILBOX_H__ */

