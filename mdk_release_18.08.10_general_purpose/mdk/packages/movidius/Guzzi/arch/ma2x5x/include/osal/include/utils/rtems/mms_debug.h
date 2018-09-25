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

#ifndef __MMS_LIBRARY_DEBUG_LINUX_H__
#define __MMS_LIBRARY_DEBUG_LINUX_H__


#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define mmsdbg_define_name(name)
#define mmsdbg_define_output                  FILE *stddbg
#define mmsdbg_config_output(std)             stddbg = std
extern FILE                                   *stddbg;

#undef mmsdbg_section
#define mmsdbg_section		              __attribute__((section("mmsdbgvarlist")))

extern void *mmsdbgvarlist;

#define TIMEVAL_STRUCT(x)                     osal_timeval x
#define TIMEVAL_READ(x)                       gettimeofday(x, 0);
#define TIMEVAL_PRINT(x) \
    (unsigned int)__tv.tv_sec, (unsigned int)__tv.tv_usec
#define TIMEVAL_SUB_PRINT(x,y) \
    ((unsigned int)(((x)->tv_sec-(y)->tv_sec)*1000000+((x)->tv_usec-(y)->tv_usec)))

//#define MMSGETTHREADID()                      ((long int)syscall(224))
//#define MMSGETTHREADID()                      getpid()
#define MMSGETTHREADID()                      pthread_self()
#define FLUSHPRINT(format, arg...)            printf(format, ##arg); fflush(stdout)

#define MMSDBGPRINT(level, format, arg...) { \
    if ((level) & DL_FATAL) { \
        printf("\033[1;5;91m[FATAL] " format "\033[0m\n", ##arg); \
    } else if ((level) & DL_ERROR) { \
        printf("\033[91m[E] " format "\033[0m\n", ##arg); \
    } else if ((level) & DL_WARNING) { \
        printf("\033[35m[W] " format "\033[0m\n", ##arg); \
    } else if ((level) & DL_FUNC) { \
        printf("\033[32m" format "\033[0m\n", ##arg); \
    } else if ((level) & DL_MESSAGE) { \
        printf("\033[36m" format "\033[0m\n", ##arg); \
    } else if ((level) & DL_PRINT) { \
        printf(format "\n", ##arg); \
    } else { \
        printf(format "\n", ##arg); \
    } \
}

#ifdef __GNUC__

#define mmsdbg(level, format, arg...) { \
    if (mmsdbgis(level)) { \
        MMSDBGPRINT(level, "%s():%d: " format, __FUNCTION__, __LINE__, ##arg); \
    } \
}

#define mmssys(level, format, arg...) { \
    if (mmsdbgis(level)) { \
        MMSDBGPRINT(level, "%s: " format " - %s", __FUNCTION__, ##arg, strerror(errno)); \
    } \
}

#define mmsinfo(level, format, arg...) { \
    if (mmsdbgis(level)) { \
        MMSDBGPRINT(level, format, ##arg); \
    } \
}

#define mmsinfot(level, format, arg...) { \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        MMSDBGPRINT(level, "%010u:%06u: " format, TIMEVAL_PRINT(&__tv), ##arg); \
    } \
}

#define mmsinfodt(level, tv1, format, arg...) { \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        MMSDBGPRINT(level, "%7d: " format, TIMEVAL_SUB_PRINT(&__tv,tv1), ##arg); \
        memcpy(tv1, &__tv, sizeof(__tv)); \
    } \
}

#define mmsdbgt(level, format, arg...) { \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        MMSDBGPRINT(level, "%4d:%010u:%06u:%s: " format, (int)MMSGETTHREADID(), \
            TIMEVAL_PRINT(&__tv), __FUNCTION__, ##arg); \
    } \
}

#define mmsdbgdt(level, tv1, format, arg...) { \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        MMSDBGPRINT(level, "%4d:%7d:%s: " format, (int)MMSGETTHREADID(), \
            (unsigned int)((__tv.tv_sec-(tv1)->tv_sec)*1000000+(__tv.tv_usec-(tv1)->tv_usec)), __FUNCTION__, ##arg); \
        memcpy(tv1, &__tv, sizeof(__tv)); \
    } \
}

#define mmsdbgadt(level, tv1, format, arg...) { \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        MMSDBGPRINT(level, "%4d:%7d:%s: " format, (int)MMSGETTHREADID(), \
            (unsigned int)((__tv.tv_sec-(tv1)->tv_sec)*1000000+(__tv.tv_usec-(tv1)->tv_usec)), __FUNCTION__, ##arg); \
    } \
}

#define mmsdbgtrace(format, arg...) { \
    MMSDBGPRINT(0, "%4d:%s:%d:%s: " format, (int)MMSGETTHREADID(), \
        __FUNCTION__, __LINE__, strerror(errno), ##arg); \
}

#define mmsdump(addr,buf) { \
    MMSDBGPRINT(0, "%08X: "mmsdbg_DUMPOCTET" - "mmsdbg_DUMPOCTET"\n", (int)addr, \
        ((char*)(buf))[ 0], ((char*)(buf))[ 1], ((char*)(buf))[ 2], ((char*)(buf))[ 3],  \
        ((char*)(buf))[ 4], ((char*)(buf))[ 5], ((char*)(buf))[ 6], ((char*)(buf))[ 7],  \
        ((char*)(buf))[ 8], ((char*)(buf))[ 9], ((char*)(buf))[10], ((char*)(buf))[11],  \
        ((char*)(buf))[12], ((char*)(buf))[13], ((char*)(buf))[14], ((char*)(buf))[15]); \
}

#define mmsdbgdump(level,str,addr,count) { \
    if (mmsdbgis(level)) { \
        int __cnt; \
        unsigned char *__dumpdata = (unsigned char*)addr; \
        for (__cnt=0;__cnt<(signed)(count);__cnt++) { \
            if ((__cnt & 0xF)==0) \
                printf("%s:%p:", str, __dumpdata); \
            printf(" %02X", *__dumpdata++); \
            if ((__cnt & 0xF)==0xF) \
                printf("\n"); \
        } \
        if ((__cnt & 0xF)!=0x00) \
            printf("\n"); \
    } \
}

#else /* ___GNUC___ */


#define mmsdbg(level, format, ...) \
    if (mmsdbgis(level)) {\
        printf("%4d:%s: " format "\n", (int)MMSGETTHREADID(), __FUNCTION__, ##__VA_ARGS__); \
    }

#define mmssys(level, format, ...) \
    if (mmsdbgis(level)) { \
        printf("%s: " format " - %s\n", __FUNCTION__, ##__VA_ARGS__, strerror(errno)); \
    }

#define mmsinfo(level, format, ...) \
    if (mmsdbgis(level)) { \
        printf( format "\n", ##__VA_ARGS__); \
    }

#define mmsinfot(level, format, ...) \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        printf( "%010u:%06u: " format "\n", TIMEVAL_PRINT(&__tv), ##__VA_ARGS__); \
    }

#define mmsinfodt(level, tv1, format, ...) \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        printf( "%7d: " format "\n", TIMEVAL_SUB_PRINT(&__tv,tv1), ##__VA_ARGS__); \
        memcpy(tv1, &__tv, sizeof(__tv)); \
    }

#define mmsdbgt(level, format, ...) \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        printf( "%4d:%010u:%06u:%s: " format "\n", (int)MMSGETTHREADID(), \
            TIMEVAL_PRINT(&__tv), __FUNCTION__, ##__VA_ARGS__); \
    }

#define mmsdbgdt(level, tv1, format, ...) \
    if (mmsdbgis(level)) { \
        osal_timeval __tv; \
        TIMEVAL_READ(&__tv); \
        printf( "%4d:%7d:%s: " format "\n", (int)MMSGETTHREADID(), \
            TIMEVAL_SUB_PRINT(&__tv,tv1), __FUNCTION__, ##__VA_ARGS__); \
        memcpy(tv1, &__tv, sizeof(__tv)); \
    }

#define mmsdbgadt(level, tv1, format, ...) \
    if (mmsdbgis(level)) { \
    osal_timeval __tv; \
    TIMEVAL_READ(&__tv); \
    printf( "%4d:%7d:%s: " format "\n", (int)MMSGETTHREADID(), \
        TIMEVAL_SUB_PRINT(&__tv,tv1), __FUNCTION__, ##__VA_ARGS__); \
    }

#define mmsdbgtrace(format, arg...) {\
      printf("%4d:%s:%d:%s: " format "\n", (int)MMSGETTHREADID(), \
          __FUNCTION__, __LINE__, strerror(errno), ##arg); \
  }

#define mmsdump(addr, buf) { \
    printf( "%08X: "mmsdbg_DUMPOCTET" - "mmsdbg_DUMPOCTET"\n", (int)addr, \
      ((char*)(buf))[ 0], ((char*)(buf))[ 1], ((char*)(buf))[ 2], ((char*)(buf))[ 3],  \
      ((char*)(buf))[ 4], ((char*)(buf))[ 5], ((char*)(buf))[ 6], ((char*)(buf))[ 7],  \
      ((char*)(buf))[ 8], ((char*)(buf))[ 9], ((char*)(buf))[10], ((char*)(buf))[11],  \
      ((char*)(buf))[12], ((char*)(buf))[13], ((char*)(buf))[14], ((char*)(buf))[15]); \
  }

#define mmsdbgdump(level, str, addr, count) { \
    if (mmsdbgis(level)) { \
      int __cnt; \
      char *__dumpdata = (char*)addr; \
      for (__cnt=0;__cnt<(signed)(count);__cnt++) { \
        if ((__cnt & 0xF)==0) printf( "%s:%08X:", str, (int)__dumpdata); \
        printf( " %02X", *__dumpdata++); \
        if ((__cnt & 0xF)==0xF) printf( "\n"); \
      } \
      if ((__cnt & 0xF)!=0x00) printf( "\n"); \
    } \
  }

#endif // __GNUC__

#endif // __MMS_LIBRARY_DEBUG_H__
