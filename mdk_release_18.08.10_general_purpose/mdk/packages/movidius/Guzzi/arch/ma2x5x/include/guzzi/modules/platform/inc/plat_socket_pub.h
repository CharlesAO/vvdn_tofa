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
* @file plat_socket_pub.h
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

#ifndef __PLAT_SOCKET_PUB_H__
#define __PLAT_SOCKET_PUB_H__



typedef enum {
    PH_SOCKET_DUMMY = 0,
    PH_SOCKET_1,
    PH_SOCKET_2,
    PH_SOCKET_3,
    PH_SOCKET_4,
    PH_SOCKET_5,
    PH_SOCKET_6,
    PH_SOCKET_MAX,
    PH_SOCKET_NONE = 0xFFFFFFFF
} PLAT_PH_SOCKET_T;



typedef enum {
    SOCKET_COMP_DUMMY,
    SOCKET_COMP_SENSOR,
    SOCKET_COMP_LENS,
    SOCKET_COMP_FLASH,
    SOCKET_COMP_NVM,
    SOCKET_COMP_MAX
} PLAT_SOCKET_COMPONENT_T;



typedef enum {
    SEN_RSRC_DUMMY,
    SEN_RSRC_POWER_1,
    SEN_RSRC_POWER_2,
    SEN_RSRC_POWER_3,
    SEN_RSRC_CLOCK,
    SEN_RSRC_ENABLE,
    SEN_RSRC_RESET,
    SEN_RSRC_XSHTDWN,
    SEN_RSRC_SYNC,
    SEN_RSRC_CCI,
    SEN_RSRC_IMGIF,
    SEN_RSRC_MAX
} PLAT_SOCKET_SENS_RSRC_TYPE_T;

typedef enum {
    DUMM_RSRC_DUMMY,
    DUMM_RSRC_POWER,
    DUMM_RSRC_MAX
} PLAT_SOCKET_DUMM_RSRC_TYPE_T;

typedef enum
{
    LENS_RSRC_DUMMY,
    LENS_RSRC_POWER,
    LENS_RSRC_CLOCK,
    LENS_RSRC_ENABLE,
    LENS_RSRC_CCI,
    LENS_RSRC_MAX
} PLAT_SOCKET_LENS_RSRC_TYPE_T;

typedef enum
{
    FLASH_RSRC_DUMMY,
    FLASH_RSRC_POWER,
    FLASH_RSRC_CLOCK,
    FLASH_RSRC_TORCH,
    FLASH_RSRC_STROBE,
    FLASH_RSRC_CCI,
    FLASH_RSRC_MAX
} PLAT_SOCKET_FLASH_RSRC_TYPE_T;

typedef enum
{
    NVM_RSRC_DUMMY,
    NVM_RSRC_POWER,
    NVM_RSRC_CLOCK,
    NVM_RSRC_ENABLE,
    NVM_RSRC_CCI,
    NVM_RSRC_MAX
} PLAT_SOCKET_NVM_RSRC_TYPE_T;



typedef enum {
    HW_RSRC_DUMMY,
    HW_RSRC_POWER,
    HW_RSRC_GPIO,
    HW_RSRC_I2C,
//TODO:    HW_RSRC_SPI,
    HW_RSRC_CLOCK,
    HW_RSRC_I2CCLOCK,
    HW_RSRC_CSI2,
//TODO:    HW_RSRC_CCP,
    HW_RSRC_MAX
} PLAT_SOCKET_HW_RSRC_TYPE_T;



typedef struct {
    PLAT_PH_SOCKET_T ph_socket;
    int              sock_comp;
    int              comp_rsrc;
    int              rsrc_type;
} plat_socket_rsrc_selector_t;


/* ========================================================================== */
/**
* plat_socket_hw_hndl_get()
*/
/* ========================================================================== */
int plat_socket_hw_hndl_get(plat_socket_rsrc_selector_t* sel, int* hw_type,
    void** hw_hndl);

#endif // __PLAT_SOCKET_PUB_H__
