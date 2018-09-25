/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   moviVectorAsTypes.h
 *  Description:   Vector Utilities Library "as type" conversion definitions
 *  --------------------------------------------------------------------------- */

#ifndef _MOVIVECTORASTYPES_H_
#define _MOVIVECTORASTYPES_H_ (1)

#ifndef __MOVICOMPILE__
# error "moviVectorAsTypes.h: this header must be included only in a file compiled with moviCompile!"
#elif defined(__shavenn__)
# error "moviVectorAsTypes.h: is not supported for SHAVE NN"
#endif


#pragma GCC system_header

#include <moviVectorTypes.h>


/* Scalars */
#define mvuAs_longlong(x)     (__builtin_astype((x),     longlong))
#define mvuAs_ulonglong(x)    (__builtin_astype((x),    ulonglong))
#define mvuAs_long(x)         (__builtin_astype((x),         long))
#define mvuAs_ulong(x)        (__builtin_astype((x),        ulong))
#define mvuAs_int(x)          (__builtin_astype((x),          int))
#define mvuAs_uint(x)         (__builtin_astype((x),         uint))
#define mvuAs_short(x)        (__builtin_astype((x),        short))
#define mvuAs_ushort(x)       (__builtin_astype((x),       ushort))
#define mvuAs_schar(x)        (__builtin_astype((x),        schar))
#define mvuAs_char(x)         (__builtin_astype((x),         char))
#define mvuAs_uchar(x)        (__builtin_astype((x),        uchar))
#define mvuAs_byte(x)         (__builtin_astype((x),         byte))
#define mvuAs_float(x)        (__builtin_astype((x),        float))
#define mvuAs_half(x)         (__builtin_astype((x),         half))

/* Cardinal 2 vectors */
#define mvuAs_longlong2(x)    (__builtin_astype((x),    longlong2))
#define mvuAs_ulonglong2(x)   (__builtin_astype((x),   ulonglong2))
#define mvuAs_long2(x)        (__builtin_astype((x),        long2))
#define mvuAs_ulong2(x)       (__builtin_astype((x),       ulong2))
#define mvuAs_int2(x)         (__builtin_astype((x),         int2))
#define mvuAs_uint2(x)        (__builtin_astype((x),        uint2))
#define mvuAs_short2(x)       (__builtin_astype((x),       short2))
#define mvuAs_ushort2(x)      (__builtin_astype((x),      ushort2))
#define mvuAs_schar2(x)       (__builtin_astype((x),       schar2))
#define mvuAs_char2(x)        (__builtin_astype((x),        char2))
#define mvuAs_uchar2(x)       (__builtin_astype((x),       uchar2))
#define mvuAs_byte2(x)        (__builtin_astype((x),        byte2))
#define mvuAs_float2(x)       (__builtin_astype((x),       float2))
#define mvuAs_half2(x)        (__builtin_astype((x),        half2))

/* Cardinal 3 vectors */
#define mvuAs_longlong3(x)    (__builtin_astype((x),    longlong3))
#define mvuAs_ulonglong3(x)   (__builtin_astype((x),   ulonglong3))
#define mvuAs_long3(x)        (__builtin_astype((x),        long3))
#define mvuAs_ulong3(x)       (__builtin_astype((x),       ulong3))
#define mvuAs_int3(x)         (__builtin_astype((x),         int3))
#define mvuAs_uint3(x)        (__builtin_astype((x),        uint3))
#define mvuAs_short3(x)       (__builtin_astype((x),       short3))
#define mvuAs_ushort3(x)      (__builtin_astype((x),      ushort3))
#define mvuAs_schar3(x)       (__builtin_astype((x),       schar3))
#define mvuAs_char3(x)        (__builtin_astype((x),        char3))
#define mvuAs_uchar3(x)       (__builtin_astype((x),       uchar3))
#define mvuAs_byte3(x)        (__builtin_astype((x),        byte3))
#define mvuAs_float3(x)       (__builtin_astype((x),       float3))
#define mvuAs_half3(x)        (__builtin_astype((x),        half3))

/* Cardinal 4 vectors */
#define mvuAs_longlong4(x)    (__builtin_astype((x),    longlong4))
#define mvuAs_ulonglong4(x)   (__builtin_astype((x),   ulonglong4))
#define mvuAs_long4(x)        (__builtin_astype((x),        long4))
#define mvuAs_ulong4(x)       (__builtin_astype((x),       ulong4))
#define mvuAs_int4(x)         (__builtin_astype((x),         int4))
#define mvuAs_uint4(x)        (__builtin_astype((x),        uint4))
#define mvuAs_short4(x)       (__builtin_astype((x),       short4))
#define mvuAs_ushort4(x)      (__builtin_astype((x),      ushort4))
#define mvuAs_schar4(x)       (__builtin_astype((x),       schar4))
#define mvuAs_char4(x)        (__builtin_astype((x),        char4))
#define mvuAs_uchar4(x)       (__builtin_astype((x),       uchar4))
#define mvuAs_byte4(x)        (__builtin_astype((x),        byte4))
#define mvuAs_float4(x)       (__builtin_astype((x),       float4))
#define mvuAs_half4(x)        (__builtin_astype((x),        half4))

/* Cardinal 8 vectors */
#define mvuAs_longlong8(x)    (__builtin_astype((x),    longlong8))
#define mvuAs_ulonglong8(x)   (__builtin_astype((x),   ulonglong8))
#define mvuAs_long8(x)        (__builtin_astype((x),        long8))
#define mvuAs_ulong8(x)       (__builtin_astype((x),       ulong8))
#define mvuAs_int8(x)         (__builtin_astype((x),         int8))
#define mvuAs_uint8(x)        (__builtin_astype((x),        uint8))
#define mvuAs_short8(x)       (__builtin_astype((x),       short8))
#define mvuAs_ushort8(x)      (__builtin_astype((x),      ushort8))
#define mvuAs_schar8(x)       (__builtin_astype((x),       schar8))
#define mvuAs_char8(x)        (__builtin_astype((x),        char8))
#define mvuAs_uchar8(x)       (__builtin_astype((x),       uchar8))
#define mvuAs_byte8(x)        (__builtin_astype((x),        byte8))
#define mvuAs_float8(x)       (__builtin_astype((x),       float8))
#define mvuAs_half8(x)        (__builtin_astype((x),        half8))

/* Cardinal 16 vectors */
#define mvuAs_longlong16(x)   (__builtin_astype((x),   longlong16))
#define mvuAs_ulonglong16(x)  (__builtin_astype((x),  ulonglong16))
#define mvuAs_long16(x)       (__builtin_astype((x),       long16))
#define mvuAs_ulong16(x)      (__builtin_astype((x),      ulong16))
#define mvuAs_int16(x)        (__builtin_astype((x),        int16))
#define mvuAs_uint16(x)       (__builtin_astype((x),       uint16))
#define mvuAs_short16(x)      (__builtin_astype((x),      short16))
#define mvuAs_ushort16(x)     (__builtin_astype((x),     ushort16))
#define mvuAs_schar16(x)      (__builtin_astype((x),      schar16))
#define mvuAs_char16(x)       (__builtin_astype((x),       char16))
#define mvuAs_uchar16(x)      (__builtin_astype((x),      uchar16))
#define mvuAs_byte16(x)       (__builtin_astype((x),       byte16))
#define mvuAs_float16(x)      (__builtin_astype((x),      float16))
#define mvuAs_half16(x)       (__builtin_astype((x),       half16))


#endif /* _MOVIVECTORASTYPES_H_ */
