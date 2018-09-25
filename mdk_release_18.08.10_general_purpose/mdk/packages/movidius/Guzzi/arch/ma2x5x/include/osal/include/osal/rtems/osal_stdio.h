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
* @file osal_stdio.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 10-Jun-2014 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef  __OSAL_STDIO_RTEMS_H__
#define  __OSAL_STDIO_RTEMS_H__

#include <stdio.h>

#define osal_printf              printf
#define osal_sprintf             sprintf
#define osal_snprintf            snprintf
#define osal_sscanf              sscanf
#define osal_fprintf             fprintf
#define osal_fopen               fopen
#define osal_fclose              fclose
#define osal_fflush              fflush
#define osal_fgets               fgets
#define osal_fseek               fseek
#define osal_ftell               ftell
#define osal_fread               fread
#define osal_fwrite              fwrite

typedef FILE                     OSAL_FILE;

#endif // __OSAL_STDIO_RTEMS_H__

