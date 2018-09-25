// -----------------------------------------------------------------------------
// Copyright (C) 2018 Movidius Ltd. All rights reserved
//
// Company          : Movidius
// Description      : Public header file for exported SIPP Framework types
//
// Notes            : #ifdefs in this file must be reflected in LEON and SHAVE
//                    compiler options
// -----------------------------------------------------------------------------

#ifndef _SIPP_SHAVESYM_H_
#define _SIPP_SHAVESYM_H_

// Shave symbols that need to be understood by leon need to be declared through "SVU_SYM" MACRO,
// as moviCompile adds a leading _ to symbol exported

#if defined(__MOVICOMPILE__)
#define SHAVE_SYM_EXPORT(x) void SVU_SYM (x)(SippFilter *fptr)
#elif defined SIPP_PC
#define SHAVE_SYM_EXPORT(x)
#else
#if SIPP_NUM_IMGS == 1
#define SHAVE_SYM_EXPORT(x) void SVU_SYM (x)(SippFilter *fptr)
#elif SIPP_NUM_IMGS == 2
#define SHAVE_SYM_EXPORT(x) void SVU_SYM0 (x)(SippFilter *fptr); \
                            void SVU_SYM1 (x)(SippFilter *fptr);
#elif SIPP_NUM_IMGS == 3
#define SHAVE_SYM_EXPORT(x) void SVU_SYM0 (x)(SippFilter *fptr); \
                            void SVU_SYM1 (x)(SippFilter *fptr); \
                            void SVU_SYM2 (x)(SippFilter *fptr);
#elif SIPP_NUM_IMGS == 4
#define SHAVE_SYM_EXPORT(x) void SVU_SYM0 (x)(SippFilter *fptr); \
                            void SVU_SYM1 (x)(SippFilter *fptr); \
                            void SVU_SYM2 (x)(SippFilter *fptr); \
                            void SVU_SYM3 (x)(SippFilter *fptr);
#elif SIPP_NUM_IMGS == 5
#define SHAVE_SYM_EXPORT(x) void SVU_SYM0 (x)(SippFilter *fptr); \
                            void SVU_SYM1 (x)(SippFilter *fptr); \
                            void SVU_SYM2 (x)(SippFilter *fptr); \
                            void SVU_SYM3 (x)(SippFilter *fptr); \
                            void SVU_SYM4 (x)(SippFilter *fptr);
#elif SIPP_NUM_IMGS == 6
#define SHAVE_SYM_EXPORT(x) void SVU_SYM0 (x)(SippFilter *fptr); \
                            void SVU_SYM1 (x)(SippFilter *fptr); \
                            void SVU_SYM2 (x)(SippFilter *fptr); \
                            void SVU_SYM3 (x)(SippFilter *fptr); \
                            void SVU_SYM4 (x)(SippFilter *fptr); \
                            void SVU_SYM5 (x)(SippFilter *fptr);
#elif SIPP_NUM_IMGS == 7
#define SHAVE_SYM_EXPORT(x) void SVU_SYM0 (x)(SippFilter *fptr); \
                            void SVU_SYM1 (x)(SippFilter *fptr); \
                            void SVU_SYM2 (x)(SippFilter *fptr); \
                            void SVU_SYM3 (x)(SippFilter *fptr); \
                            void SVU_SYM4 (x)(SippFilter *fptr); \
                            void SVU_SYM5 (x)(SippFilter *fptr); \
                            void SVU_SYM6 (x)(SippFilter *fptr);
#elif SIPP_NUM_IMGS == 8
#define SHAVE_SYM_EXPORT(x) void SVU_SYM0 (x)(SippFilter *fptr); \
                            void SVU_SYM1 (x)(SippFilter *fptr); \
                            void SVU_SYM2 (x)(SippFilter *fptr); \
                            void SVU_SYM3 (x)(SippFilter *fptr); \
                            void SVU_SYM4 (x)(SippFilter *fptr); \
                            void SVU_SYM5 (x)(SippFilter *fptr); \
                            void SVU_SYM6 (x)(SippFilter *fptr); \
                            void SVU_SYM7 (x)(SippFilter *fptr);
#endif
#endif

#endif /* _SIPP_SHAVESYM_H_ */
