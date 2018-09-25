/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   moviVectorConvert.h
 *  Description:   Vector Utilities Library conversion definitions
 *  --------------------------------------------------------------------------- */

#ifndef _MOVIVECTORCONVERT_H_
#define _MOVIVECTORCONVERT_H_ (1)

#ifndef __MOVICOMPILE__
# error "moviVectorConvert.h: this header must be included only in a file compiled with moviCompile!"
#elif defined(__shavenn__)
# error "moviVectorConvert.h: is not supported for SHAVE NN"
#endif


#pragma GCC system_header

#include <moviVectorTypes.h>


/* Scalars */
#define mvuConvert_longlong(x)     ((longlong)(x))
#define mvuConvert_ulonglong(x)    ((ulonglong)(x))
#define mvuConvert_long(x)         ((long)(x))
#define mvuConvert_ulong(x)        ((ulong)(x))
#define mvuConvert_int(x)          ((int)(x))
#define mvuConvert_uint(x)         ((uint)(x))
#define mvuConvert_short(x)        ((short)(x))
#define mvuConvert_ushort(x)       ((ushort)(x))
#define mvuConvert_schar(x)        ((schar)(x))
#define mvuConvert_char(x)         ((char)(x))
#define mvuConvert_uchar(x)        ((uchar)(x))
#define mvuConvert_byte(x)         ((byte)(x))
#define mvuConvert_float(x)        ((float)(x))
#define mvuConvert_half(x)         ((half)(x))

/* Cardinal 2 vectors */
#define mvuConvert_longlong2(x)    (__builtin_convertvector((x),    longlong2))
#define mvuConvert_ulonglong2(x)   (__builtin_convertvector((x),   ulonglong2))
#define mvuConvert_long2(x)        (__builtin_convertvector((x),        long2))
#define mvuConvert_ulong2(x)       (__builtin_convertvector((x),       ulong2))
#define mvuConvert_int2(x)         (__builtin_convertvector((x),         int2))
#define mvuConvert_uint2(x)        (__builtin_convertvector((x),        uint2))
#define mvuConvert_short2(x)       (__builtin_convertvector((x),       short2))
#define mvuConvert_ushort2(x)      (__builtin_convertvector((x),      ushort2))
#define mvuConvert_schar2(x)       (__builtin_convertvector((x),       schar2))
#define mvuConvert_char2(x)        (__builtin_convertvector((x),        char2))
#define mvuConvert_uchar2(x)       (__builtin_convertvector((x),       uchar2))
#define mvuConvert_byte2(x)        (__builtin_convertvector((x),        byte2))
#define mvuConvert_float2(x)       (__builtin_convertvector((x),       float2))
#define mvuConvert_half2(x)        (__builtin_convertvector((x),        half2))

/* Cardinal 3 vectors */
#define mvuConvert_longlong3(x)    (__builtin_convertvector((x),    longlong3))
#define mvuConvert_ulonglong3(x)   (__builtin_convertvector((x),   ulonglong3))
#define mvuConvert_long3(x)        (__builtin_convertvector((x),        long3))
#define mvuConvert_ulong3(x)       (__builtin_convertvector((x),       ulong3))
#define mvuConvert_int3(x)         (__builtin_convertvector((x),         int3))
#define mvuConvert_uint3(x)        (__builtin_convertvector((x),        uint3))
#define mvuConvert_short3(x)       (__builtin_convertvector((x),       short3))
#define mvuConvert_ushort3(x)      (__builtin_convertvector((x),      ushort3))
#define mvuConvert_schar3(x)       (__builtin_convertvector((x),       schar3))
#define mvuConvert_char3(x)        (__builtin_convertvector((x),        char3))
#define mvuConvert_uchar3(x)       (__builtin_convertvector((x),       uchar3))
#define mvuConvert_byte3(x)        (__builtin_convertvector((x),        byte3))
#define mvuConvert_float3(x)       (__builtin_convertvector((x),       float3))
#define mvuConvert_half3(x)        (__builtin_convertvector((x),        half3))

/* Cardinal 4 vectors */
#define mvuConvert_longlong4(x)    (__builtin_convertvector((x),    longlong4))
#define mvuConvert_ulonglong4(x)   (__builtin_convertvector((x),   ulonglong4))
#define mvuConvert_long4(x)        (__builtin_convertvector((x),        long4))
#define mvuConvert_ulong4(x)       (__builtin_convertvector((x),       ulong4))
#define mvuConvert_int4(x)         (__builtin_convertvector((x),         int4))
#define mvuConvert_uint4(x)        (__builtin_convertvector((x),        uint4))
#define mvuConvert_short4(x)       (__builtin_convertvector((x),       short4))
#define mvuConvert_ushort4(x)      (__builtin_convertvector((x),      ushort4))
#define mvuConvert_schar4(x)       (__builtin_convertvector((x),       schar4))
#define mvuConvert_char4(x)        (__builtin_convertvector((x),        char4))
#define mvuConvert_uchar4(x)       (__builtin_convertvector((x),       uchar4))
#define mvuConvert_byte4(x)        (__builtin_convertvector((x),        byte4))
#define mvuConvert_float4(x)       (__builtin_convertvector((x),       float4))
#define mvuConvert_half4(x)        (__builtin_convertvector((x),        half4))

/* Cardinal 8 vectors */
#define mvuConvert_longlong8(x)    (__builtin_convertvector((x),    longlong8))
#define mvuConvert_ulonglong8(x)   (__builtin_convertvector((x),   ulonglong8))
#define mvuConvert_long8(x)        (__builtin_convertvector((x),        long8))
#define mvuConvert_ulong8(x)       (__builtin_convertvector((x),       ulong8))
#define mvuConvert_int8(x)         (__builtin_convertvector((x),         int8))
#define mvuConvert_uint8(x)        (__builtin_convertvector((x),        uint8))
#define mvuConvert_short8(x)       (__builtin_convertvector((x),       short8))
#define mvuConvert_ushort8(x)      (__builtin_convertvector((x),      ushort8))
#define mvuConvert_schar8(x)       (__builtin_convertvector((x),       schar8))
#define mvuConvert_char8(x)        (__builtin_convertvector((x),        char8))
#define mvuConvert_uchar8(x)       (__builtin_convertvector((x),       uchar8))
#define mvuConvert_byte8(x)        (__builtin_convertvector((x),        byte8))
#define mvuConvert_float8(x)       (__builtin_convertvector((x),       float8))
#define mvuConvert_half8(x)        (__builtin_convertvector((x),        half8))

/* Cardinal 16 vectors */
#define mvuConvert_longlong16(x)   (__builtin_convertvector((x),   longlong16))
#define mvuConvert_ulonglong16(x)  (__builtin_convertvector((x),  ulonglong16))
#define mvuConvert_long16(x)       (__builtin_convertvector((x),       long16))
#define mvuConvert_ulong16(x)      (__builtin_convertvector((x),      ulong16))
#define mvuConvert_int16(x)        (__builtin_convertvector((x),        int16))
#define mvuConvert_uint16(x)       (__builtin_convertvector((x),       uint16))
#define mvuConvert_short16(x)      (__builtin_convertvector((x),      short16))
#define mvuConvert_ushort16(x)     (__builtin_convertvector((x),     ushort16))
#define mvuConvert_schar16(x)      (__builtin_convertvector((x),      schar16))
#define mvuConvert_char16(x)       (__builtin_convertvector((x),       char16))
#define mvuConvert_uchar16(x)      (__builtin_convertvector((x),      uchar16))
#define mvuConvert_byte16(x)       (__builtin_convertvector((x),       byte16))
#define mvuConvert_float16(x)      (__builtin_convertvector((x),      float16))
#define mvuConvert_half16(x)       (__builtin_convertvector((x),       half16))


#endif /* _MOVIVECTORCONVERT_H_ */
