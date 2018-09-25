///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Miscellaneous FLIC defines
///

#ifndef __FLIC_DEFS_H__
#define __FLIC_DEFS_H__

#define OK  0 //typical posix/rtems SUCCESS

#ifndef NULL
#define NULL 0
#endif

//=============================================
//Align/Section macros
#ifndef ALIGNED
#define ALIGNED(x) __attribute__((aligned(x)))
#endif

#ifndef SECTION
#define SECTION(x) __attribute__((section(x)))
#endif

//Used for Receive calls
#define FLIC_WU __attribute__((warn_unused_result))

//align size to 64Bytes (Leon D-cache line = 256 bits)
#define DCALGN(x) (((x) + 63) & (~63))

//=============================================
//Timeouts

//Restriction: all rel TS should be less than this
#define REL_TS_THR (365*24*60*60) //~1 year in seconds

//typespec type
typedef enum{
 REL = 0,
 ABS = 1
}TsType;

#ifndef ONE_SEND_TIMEOUT
#define ONE_SEND_TIMEOUT 1
#endif

//=============================================
//time_t is seen as signed integer,
//so (time_t)-1 would be 0xFFFFFFFF = negative time, thus immediate return.
//Therefore, mask sign:
#define TIME_MAX (((time_t)-1) & (~(((time_t)1)<<(sizeof(time_t)*8-1))))

//=============================================
//Maximum system-wide unique IDs that
//have a textual description associated
#ifndef MAX_IDS
#define MAX_IDS 64
#endif

//Invoke NamedId constructor with self name
#define FLIC_ID(x) x(#x)

//=============================================
//Default allocators section name & size
#ifndef DEF_POOL_SECT
#define DEF_POOL_SECT ".ddr.bss"
#endif

#ifndef CMX_POOL_SECT
#define CMX_POOL_SECT ".cmx_direct.data"
#endif

#ifndef DEF_POOL_SZ
#define DEF_POOL_SZ 32
#endif

#ifndef CMX_POOL_SZ
#define CMX_POOL_SZ 32
#endif

//=============================================
//Default max number of plugins per pipe
#ifndef MAX_PLUGS_PER_PIPE
#define MAX_PLUGS_PER_PIPE 32
#endif

//Max number of receivers per sender
#ifndef MAX_RECS_PER_SENDER
#define MAX_RECS_PER_SENDER 8
#endif

//Max number of senders+receivers per Plugin
#ifndef MAX_IOS_PER_PLUG
#define MAX_IOS_PER_PLUG 16
#endif

#define EMPTY_SLOT 0
#define INVALID_IDX -1 //invalid array pos

//Log Pipe setup operations: AddPlug, Start, Stop,
// Link filter ...
#ifndef FLIC_PLOG
#define FLIC_PLOG 0
#endif

//Log Beginning of Push/Pop
#ifndef FLIC_OP_BEGIN_LOG
#define FLIC_OP_BEGIN_LOG 0
#endif

//Log End of Push/Pop
#ifndef FLIC_OP_END_LOG
#define FLIC_OP_END_LOG 1
#endif

//Use optimized PoPtr IPC handling
#ifndef USE_OPT_POPTR
#define USE_OPT_POPTR 1
#endif

//Enable inter-Leon barriers
#ifndef FLIC_RMI_EN_BARR
#define FLIC_RMI_EN_BARR 1
#endif

#endif
