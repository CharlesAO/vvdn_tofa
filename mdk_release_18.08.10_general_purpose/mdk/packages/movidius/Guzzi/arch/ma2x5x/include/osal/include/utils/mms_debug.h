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
* @file mms_debug.h
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
#ifndef __MMS_LIBRARY_DEBUG_H__
#define __MMS_LIBRARY_DEBUG_H__


#include <utils/profile/profile.h>
#include <utils/profile/profile_ids.h>

#define DL_LAYER(x)    (1 << (x))

#define DL_FATAL       DL_LAYER(0)
#define DL_ERROR       DL_LAYER(1)
#define DL_WARNING     DL_LAYER(2)
#define DL_FUNC        DL_LAYER(3)
#define DL_MESSAGE     DL_LAYER(4)
#define DL_PRINT       DL_LAYER(5)

#define DL_LAYER0      DL_LAYER(6)
#define DL_LAYER1      DL_LAYER(7)
#define DL_LAYER2      DL_LAYER(8)
#define DL_LAYER3      DL_LAYER(9)
#define DL_LAYER4      DL_LAYER(10)
#define DL_LAYER5      DL_LAYER(11)
#define DL_LAYER6      DL_LAYER(12)
#define DL_LAYER7      DL_LAYER(13)
#define DL_LAYER8      DL_LAYER(14)
#define DL_LAYER9      DL_LAYER(15)
#define DL_LAYER10     DL_LAYER(16)
#define DL_LAYER11     DL_LAYER(17)
#define DL_LAYER12     DL_LAYER(18)
#define DL_LAYER13     DL_LAYER(19)
#define DL_LAYER14     DL_LAYER(20)
#define DL_LAYER15     DL_LAYER(21)
#define DL_LAYER16     DL_LAYER(22)
#define DL_LAYER17     DL_LAYER(23)
#define DL_LAYER18     DL_LAYER(24)
#define DL_LAYER19     DL_LAYER(25)
#define DL_LAYER20     DL_LAYER(26)
#define DL_LAYER21     DL_LAYER(27)
#define DL_LAYER22     DL_LAYER(28)
#define DL_LAYER23     DL_LAYER(29)

#define DL_PROFILE     DL_LAYER(30)

#define DL_FULL        (DL_ERROR | DL_FATAL | DL_PROFILE)
#define DL_DEFAULT     (DL_ERROR | DL_FATAL | DL_PROFILE)
#define DL_LAYERS      (DL_ERROR | DL_FATAL | DL_PROFILE)

//Common definitions
#define mmsdbg_PRINTLLC(x, n)            (((unsigned char *)(x))[n])
#define mmsdbg_DUMPOCTET                "%02X %02X %02X %02X %02X %02X %02X %02X"
#define mmsdbg_DUMPLL                   "%016llX"
#define mmsdbg_PRINTL(x)                (*((unsigned long long *)(x)))

struct mmsdbg_dynamic_variable_register {
    char  magic[16];
    int   group;
    int  *var;
    char *key;
    char *comment;
};

#define MMSDBG_MAGIC_CODE   "MMSDBGVariable"

#define mmsdbg_section

#define mmsdbg_define_variable(_variable_, _default_value_, _group_, _key_, _comment_) \
    static int _variable_ = _default_value_; \
    struct mmsdbg_dynamic_variable_register \
        mmsdbg_reg_##_variable_ mmsdbg_section = { \
            .magic = { MMSDBG_MAGIC_CODE }, \
            .group = _group_, \
            .var   = &_variable_, \
            .key   = _key_, \
            .comment = _comment_, \
    };

#define mmsdbg_declare_variable(_variable_) \
    extern struct mmsdbg_dynamic_variable_register mmsdbg_reg_##_variable_;

#define mmsdbg_use_variable(_variable_) \
    (*(mmsdbg_reg_##_variable_.var))

#ifdef __MMS_DEBUG__

#define mmsdbgis(level)                       ((level) & MMSDEBUGLEVEL)
#define mmsdbglup(level)                      (MMSDEBUGLEVEL) |= (level)
#define mmsdbgldown(level)                    (MMSDEBUGLEVEL) &= (~(level))

#if  defined(___WINDOWS___)
#include "windows/mms_debug.h"
#elif defined(___ANDROID___)
#include "android/mms_debug.h"
#elif defined(___LINUX___)
#include "linux/mms_debug.h"
#elif defined(___RTEMS___)
#include "rtems/mms_debug.h"
#elif defined(___DSPBIOS___)
#include "dspbios/mms_debug.h"
#else
#include "dummy/mms_debug.h"
#endif

#else // __MMS_DEBUG__

#define mmsdbgis(level)                       (0)
#define mmsdbglup(level)
#define mmsdbgldown(level)

#include "dummy/mms_debug.h"

#endif // __MMS_DEBUG__

#endif // __MMS_LIBRARY_DEBUG_H__
