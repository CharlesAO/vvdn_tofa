//ContextDefinition here

#include "../include/theDynContext.h"

#ifndef APPENTRY
#error "Please define an entry point for your shave application"
//#define APPENTRY InvalidEntry
#define APPENTRY Entry
#endif

#ifndef CTORSARRAY
#error "Please define a valid ctors array initialize function"
#define CTORSARRAY INVALID_ARRAY
#endif

#ifndef INITARRAYEND
#error "Please define a valid init_array_end"
#define INITARRAYEND INVALID_INITARRAYEND
#endif

#ifndef DTORSARRAY
#error "Please define a valid dtors array initialize function"
#define CTORSARRAY INVALID_ARRAY
#endif

#ifndef FINIARRAYEND
#error "Please define a valid fini_array_end"
#define INITARRAYEND INVALID_INITARRAYEND
#endif
#ifndef GRPENTRY
#error "Please define an group entry point for your shave application"
//#define GRPENTRY InvalidEntry
#define GRPENTRY BOGUS___AllTimeEntry
#endif

#ifndef APPHEAPSIZE
#error "Please define a HEAP size for your shave application"
#define APPHEAPSIZE 0
#endif

#ifndef APPSTACKSIZE
#error "Please define a STACK size for your shave application"
#define APPSTACKSIZE 0
#endif

#ifndef APPDYNCONTEXTNAME
#error "Please define a dyncontext name for your shave application"
#define APPDYNCONTEXTNAME appContextData
#endif

#ifndef SHAVEAPPNAME
#error "PLease define a name for this application"
#define SHAVEAPPNAME InvalidName
#endif

#ifndef HEAP_INIT_VAL
#error "PLease define a heap init value"
#define HEAP_INIT_VAL DYNCONTEXT_HEAP_INVALID_VAL
#endif

#ifndef APPDYNBSSDATASECTION
#error "Please define a bss data section start for your shave application"
#define APPDYNBSSDATASECTION ((uint64_t *)NULL)
#endif

#ifndef APPDYNBSSDATASECTIONEND
#error "Please define a bss data section end for your shave application"
#define APPDYNBSSDATASECTIONEND ((uint64_t *)NULL)
#endif

#ifndef APPDYNDATASECTION
#error "Please define a shvdlib with data for your shave application"
#define APPDYNDATASECTION 0
#endif

#ifndef APPDYNDATASECTIONSIZE
#error "Please define a size for your shave application data section"
#define APPDYNDATASECTIONSIZE 0
#endif

#ifndef APPGROUPDYNBSSDATASECTION
#error "Please define a group bss data section start for your shave application group"
#define APPGROUPDYNDATASECTION ((uint64_t *)NULL)
#endif

#ifndef APPGROUPDYNBSSDATASECTIONEND
#error "Please define a group bss data section end for your shave application group"
#define APPGROUPDYNBSSDATASECTIONEND ((uint64_t *)NULL)
#endif

#ifndef APPGROUPDYNDATASECTION
#error "Please define a group shvdlib with data for your shave application"
#define APPGROUPDYNDATASECTION 0
#endif

#ifndef APPGROUPDYNDATASECTIONSIZE
#error "Please define a group data size for your shave application"
#define APPGROUPDYNDATASECTIONSIZE ZZZZZZZZZ_size
#endif

#ifndef APPCRITCMXTEXTSECTIONSIZE
#error "Please define a size for your shave application cmx critical code section"
#define APPCRITCMXTEXTSECTIONSIZE ZZZZZZZZZ_CMX_size
#endif
 
#ifndef APPIAT
#error "Iat table not defined"
#define APPIAT NULL
#endif

#ifndef APPIATNAMES
#error "Iat table not defined"
#define APPIATNAMES NULL
#endif

#ifndef APPIATGROUP
#error "Iat table not defined"
#define APPIAT NULL
#endif

#ifndef APPIATNAMESGROUP
#error "Iat table not defined"
#define APPIATNAMES NULL
#endif

extern unsigned char CTORSARRAY[];
extern unsigned char INITARRAYEND[];
extern unsigned char DTORSARRAY[];
extern unsigned char FINIARRAYEND[];
extern unsigned char APPENTRY[];
extern unsigned char GRPENTRY[];
extern uint64_t APPDYNBSSDATASECTION[];
extern uint64_t APPDYNBSSDATASECTIONEND[];
extern unsigned char APPDYNDATASECTION[];
extern uint64_t APPGROUPDYNBSSDATASECTION[];
extern uint64_t APPGROUPDYNBSSDATASECTIONEND[];
extern unsigned char APPGROUPDYNDATASECTION[];
extern unsigned char APPIAT[];
extern unsigned char APPIATNAMES[];
extern unsigned char APPIATGROUP[];
extern unsigned char APPIATNAMESGROUP[];

_ExecutionContext_t __attribute__((section(".shvX." SHAVEAPPNAME ".S.moduledata"))) APPEXECCONTEXTARRAY[TOTAL_NUM_SHAVES]={
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL},
        {.moviprof_data = NULL}
};

static struct DynamicContextInstances_elm __attribute__((section(".shvX." SHAVEAPPNAME ".S.moduledata"))) APPDYNCONTEXTPRIVATENAME={
        /* Initialize group data pools usage to 0. No initial group space allocated. */
        .GrpDataPools           = {(unsigned char*)0x0,},
        .GrpDataPoolsStart      = {(unsigned char*)0x0,},
        /* Initialize heap required pools to 0. No initial heap space allocated. */
        .HeapPools              = {(unsigned char*)0x0,},
        .HeapPoolsStart         = {(unsigned char*)0x0,},
        .appInstances           = 0,
        /*Round the cmx critical section size to nearest 1K because SHAVE windows are limited to such address types*/
};

DynamicContext_t __attribute__((section(".shvX." SHAVEAPPNAME ".S.moduledata"))) APPDYNCONTEXTNAME={
        .crtContextInfo         = (_ExecutionContext_t*)APPEXECCONTEXTARRAY,
        .ctors_start            = (_TorFn_t*)&CTORSARRAY,
        .ctors_end              = (_TorFn_t*)&INITARRAYEND,
        .dtors_start            = (_TorFn_t*)&DTORSARRAY,
        .dtors_end              = (_TorFn_t*)&FINIARRAYEND,
        .heap_size              = (unsigned int) APPHEAPSIZE,
        .stack_size             = (unsigned int) APPSTACKSIZE,
        .entryPoint             = APPENTRY,
        .instancesData          = (DynamicContextInstancesPtr)&APPDYNCONTEXTPRIVATENAME,
        .groupEntryPoint        = (uint32_t) GRPENTRY,
        .appdynbssdatastart     = APPDYNBSSDATASECTION,
        .appdynbssdataend       = APPDYNBSSDATASECTIONEND,
        .appdyndata             = APPDYNDATASECTION,
        .appdyndatasize         = APPDYNDATASECTIONSIZE,
        .groupappdynbssdatastart= APPGROUPDYNBSSDATASECTION,
        .groupappdynbssdataend  = APPGROUPDYNBSSDATASECTIONEND,
        .groupappdyndata        = APPGROUPDYNDATASECTION,
        .groupappdyndatasize    = APPGROUPDYNDATASECTIONSIZE,
        .initHeap               = (DYNCONTEXT_HEAP_ACTION_TYPE) HEAP_INIT_VAL,
        .reentrant              = (DYNCONTEXT_APP_REENTRANT_TYPE) DYNCONTEXT_APP_NOT_RENTRANT,
        .cmxCriticalCodeSize    = ((((unsigned int)APPCRITCMXTEXTSECTIONSIZE)+1024u) & (~1023u)),
        .iat                    = (void*)APPIAT,
        .iatnames               = (void*)APPIATNAMES,
        .iat_group              = (void*)APPIATGROUP,
        .iatnames_group         = (void*)APPIATNAMESGROUP,
};
