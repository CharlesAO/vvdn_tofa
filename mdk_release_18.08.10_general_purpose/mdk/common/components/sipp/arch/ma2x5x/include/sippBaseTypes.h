// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : Public header file for SIPP base types
//                    Aim to replace with a generic moviTypes.h file if one exists
//
// -----------------------------------------------------------------------------

#ifndef _SIPP_BASE_TYPES_H_
#define _SIPP_BASE_TYPES_H_

//////////////////////////////////////////////////////////////
// SIPP Base Types

#ifdef SIPP_USE_OWN_BASETYPES
// till MDK people wake up, I put these defs here...
// they should be in mvtypes
typedef unsigned long long UInt64;
typedef unsigned int       UInt32;
typedef unsigned short     UInt16;
typedef unsigned char      UInt8;

typedef          int       Int32;
typedef          short     Int16;
typedef          char      Int8;

#ifndef __cplusplus
typedef          u8        bool;
#endif
#else

#ifdef MYRIAD2
#include <mv_types.h>
#if SIPP_RTOS == SIPP_NO_RTOS
#ifndef __cplusplus
typedef          u8        bool;
#endif
#endif
#else
// For the c-model, some HW model header files are incompatible with mv_types.h
#include <stdint.h>
typedef  uint8_t           u8;
typedef   int8_t           s8;
typedef uint16_t           u16;
typedef  int16_t           s16;
typedef uint32_t           u32;
typedef  int32_t           s32;
typedef uint64_t           u64;
typedef  int64_t           s64;

typedef    float          fp32;

#ifndef __cplusplus
typedef u8                 bool;
#endif
#endif
// Still define the 'old' basetypes as they have been exported to many existing
// applications and filter wrappers
typedef uint64_t           UInt64;
typedef uint32_t           UInt32;
typedef uint16_t           UInt16;
typedef uint8_t            UInt8;

typedef int32_t            Int32;
typedef int16_t            Int16;
typedef int8_t             Int8;

#endif



////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////
#if SIPP_RTOS == SIPP_NO_RTOS
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif

#endif /* _SIPP_BASE_TYPES_H_ */

