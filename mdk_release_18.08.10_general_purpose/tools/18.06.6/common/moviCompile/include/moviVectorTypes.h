/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   moviVectorTypes.h
 *  Description:   Vector Utilities Library type definitions
 *  --------------------------------------------------------------------------- */

#ifndef _MOVIVECTORTYPES_H_
#define _MOVIVECTORTYPES_H_ (1)

#ifndef __MOVICOMPILE__
# error "moviVectorTypes.h: this header must be included only in a file compiled with moviCompile!"
#endif


#pragma GCC system_header

#include <stdint.h>


#ifndef __openclc__
/*  The 'half' or 16-bit Floating-Point type - this is a keyword in OpenCL */
typedef short float half;
#endif /* __openclc__ */


/*  Integer vector types */
/*  16 x 64-bit */
typedef int64_t          longlong16 __attribute__((ext_vector_type(16)));
typedef uint64_t        ulonglong16 __attribute__((ext_vector_type(16)));

/*  8 x 64-bit */
typedef int64_t           longlong8 __attribute__((ext_vector_type(8)));
typedef uint64_t         ulonglong8 __attribute__((ext_vector_type(8)));

/*  4 x 64-bit */
typedef int64_t           longlong4 __attribute__((ext_vector_type(4)));
typedef uint64_t         ulonglong4 __attribute__((ext_vector_type(4)));

/*  3 x 64-bit */
typedef int64_t           longlong3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
typedef uint64_t         ulonglong3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));

/*  2 x 64-bit */
typedef int64_t           longlong2 __attribute__((ext_vector_type(2)));
typedef uint64_t         ulonglong2 __attribute__((ext_vector_type(2)));

/*  16 x 32-bit */
typedef signed long          long16 __attribute__((ext_vector_type(16)));
typedef unsigned long       ulong16 __attribute__((ext_vector_type(16)));

/*  8 x 32-bit */
typedef signed long           long8 __attribute__((ext_vector_type(8)));
typedef unsigned long        ulong8 __attribute__((ext_vector_type(8)));

/*  4 x 32-bit */
typedef signed long           long4 __attribute__((ext_vector_type(4)));
typedef unsigned long        ulong4 __attribute__((ext_vector_type(4)));

/*  3 x 32-bit */
typedef signed long           long3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
typedef unsigned long        ulong3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));

/*  2 x 32-bit */
typedef signed long           long2 __attribute__((ext_vector_type(2)));
typedef unsigned long        ulong2 __attribute__((ext_vector_type(2)));

/*  16 x 32-bit */
typedef signed int            int16 __attribute__((ext_vector_type(16)));
typedef unsigned int         uint16 __attribute__((ext_vector_type(16)));

/*  8 x 32-bit */
typedef signed int             int8 __attribute__((ext_vector_type(8)));
typedef unsigned int          uint8 __attribute__((ext_vector_type(8)));

/*  4 x 32-bit */
typedef signed int             int4 __attribute__((ext_vector_type(4)));
typedef unsigned int          uint4 __attribute__((ext_vector_type(4)));

/*  3 x 32-bit */
typedef signed int             int3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
typedef unsigned int          uint3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));

/*  2 x 32-bit */
typedef signed int             int2 __attribute__((ext_vector_type(2)));
typedef unsigned int          uint2 __attribute__((ext_vector_type(2)));

/*  16 x 16-bit */
typedef signed short        short16 __attribute__((ext_vector_type(16)));
typedef unsigned short     ushort16 __attribute__((ext_vector_type(16)));

/*  8 x 16-bit */
typedef signed short         short8 __attribute__((ext_vector_type(8)));
typedef unsigned short      ushort8 __attribute__((ext_vector_type(8)));

/*  4 x 16-bit */
typedef signed short         short4 __attribute__((ext_vector_type(4)));
typedef unsigned short      ushort4 __attribute__((ext_vector_type(4)));

/*  3 x 16-bit */
typedef signed short         short3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
typedef unsigned short      ushort3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));

/*  2 x 16-bit */
typedef signed short         short2 __attribute__((ext_vector_type(2)));
typedef unsigned short      ushort2 __attribute__((ext_vector_type(2)));

/*  16 x 8-bit */
typedef signed char         schar16 __attribute__((ext_vector_type(16)));
typedef char                 char16 __attribute__((ext_vector_type(16)));
typedef unsigned char       uchar16 __attribute__((ext_vector_type(16)));

/*  8 x 8-bit */
typedef signed char          schar8 __attribute__((ext_vector_type(8)));
typedef char                  char8 __attribute__((ext_vector_type(8)));
typedef unsigned char        uchar8 __attribute__((ext_vector_type(8)));

/*  4 x 8-bit */
typedef signed char          schar4 __attribute__((ext_vector_type(4)));
typedef char                  char4 __attribute__((ext_vector_type(4)));
typedef unsigned char        uchar4 __attribute__((ext_vector_type(4)));

/*  3 x 8-bit */
typedef signed char          schar3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
typedef char                  char3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
typedef unsigned char        uchar3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));

/*  2 x 8-bit */
typedef signed char          schar2 __attribute__((ext_vector_type(2)));
typedef char                  char2 __attribute__((ext_vector_type(2)));
typedef unsigned char        uchar2 __attribute__((ext_vector_type(2)));


/*  Floating-Point vector types */
/*  16 x 32-bit */
typedef float               float16 __attribute__((ext_vector_type(16)));
/*  8 x 32-bit */
typedef float                float8 __attribute__((ext_vector_type(8)));
/*  4 x 32-bit */
typedef float                float4 __attribute__((ext_vector_type(4)));
/*  3 x 32-bit */
typedef float                float3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
/*  2 x 32-bit */
typedef float                float2 __attribute__((ext_vector_type(2)));

/*  16 x 16-bit */
typedef half                 half16 __attribute__((ext_vector_type(16)));
/*  8 x 16-bit */
typedef half                  half8 __attribute__((ext_vector_type(8)));
/*  4 x 16-bit */
typedef half                  half4 __attribute__((ext_vector_type(4)));
/*  3 x 16-bit */
typedef half                  half3 __attribute__((ext_vector_type(3), deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
/*  2 x 16-bit */
typedef half                  half2 __attribute__((ext_vector_type(2)));

/*  Matrix vector types */
/*  4x4 integer matrix */
typedef struct int4x4
{
    int4 rows[4];
} int4x4;

/*  4x4 unsigned integer matrix */
typedef struct uint4x4
{
    uint4 rows[4];
} uint4x4;

/*  4x4 long matrix */
typedef struct long4x4
{
    long4 rows[4];
} long4x4;

/*  4x4 unsigned long matrix */
typedef struct ulong4x4
{
    ulong4 rows[4];
} ulong4x4;

/*  4x4 float matrix */
typedef struct float4x4
{
    float4 rows[4];
} float4x4;

/*  8x8 half matrix */
typedef struct half8x8
{
    half8 rows[8];
} half8x8;

/*  8x8 short matrix  */
typedef struct short8x8
{
    short8 rows[8];
} short8x8;

/*  8x8 unsigned short matrix  */
typedef struct ushort8x8
{
    ushort8 rows[8];
} ushort8x8;


/*  Alternative names */
typedef uchar16        byte16;
typedef uchar8         byte8;
typedef uchar4         byte4;
typedef uchar3         byte3 __attribute__((deprecated("Warning! See 'moviCompile.pdf' Section 6.3.2.5.1 for details")));
typedef uchar2         byte2;
typedef unsigned char  byte;

typedef signed char    schar;
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;
typedef int64_t        longlong;
typedef uint64_t       ulonglong;


#endif /* _MOVIVECTORTYPES_H_ */
