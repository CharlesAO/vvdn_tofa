///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup
/// @defgroup
/// @ingroup
/// @{
/// @brief Export type definitions for Context structure. These are used by Shave Dynamic Loading function.
///
///
///

#ifndef SUB_MODULE_H
#define SUB_MODULE_H

#include "DrvRegUtilsDefines.h"
#include "DrvSvuDefines.h"
#include "stdlib.h"
#include "sys/shave_system.h"
#include "sys/shave_exitcodes.h"

#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
#include <memManagerApi.h>
#endif

#define TOKEN_PASTE_INTERN(APP) APP ## X_ModuleData
#define MODULE_DATA_INTERN(APP) TOKEN_PASTE_INTERN(APP)

#define TOKEN_PASTE(APP) APP ## X_ModuleData
#define MODULE_DATA(APP) TOKEN_PASTE(APP)

typedef enum {
    DYNCONTEXT_HEAP_NOINIT = 0,
    DYNCONTEXT_HEAP_INIT = 1,
    DYNCONTEXT_HEAP_INVALID_VAL = 3
} DYNCONTEXT_HEAP_ACTION_TYPE;

typedef enum {
    DYNCONTEXT_APP_NOT_RENTRANT = 0,
    DYNCONTEXT_APP_REENTRANT = 1
} DYNCONTEXT_APP_REENTRANT_TYPE;

typedef enum {
    MISA_DECOUPLED = 5001,
    MISA_FULLY_COUPLED
} MISA_PARADIGM_TYPE;

typedef u32 swcShaveUnit_t;
typedef u32 ParadigmSpecificEntry;

struct DynamicContextInstances_elm {
    /* This field retains the allocated library group data addresses
     * for each SHAVE. These should be freed if at any point in time we want to reset
     * usage of a particular app */
    unsigned char *GrpDataPools[TOTAL_NUM_SHAVES];
    unsigned char *GrpDataPoolsStart[TOTAL_NUM_SHAVES];
    /* This field retains the extra allocated library group data addresses
     * for each SHAVE. These should be freed if at any point in time we want to reset
     * usage of a particular app */
    unsigned char *HeapPools[TOTAL_NUM_SHAVES];
    unsigned char *HeapPoolsStart[TOTAL_NUM_SHAVES];
    /*This field stores the number of configured instances*/
    uint32_t appInstances;
    /* This field stores the shave allocation map */
    swcShaveUnit_t shaveList[TOTAL_NUM_SHAVES];
};

typedef struct DynamicContextInstances_elm* DynamicContextInstancesPtr;

typedef struct DynamicContext_elm {
    _ExecutionContext_t* crtContextInfo;
    _TorFn_t* ctors_start; 	/* '__init_array_start' to allow loading the application constructors */
    _TorFn_t* ctors_end;	/* '__init_array_end' symbol marks the end of the application .ctors section */
    _TorFn_t* dtors_start; 	/* '__fini_array_start' to allow loading the application destructors */
    _TorFn_t* dtors_end; 	/* '__fini_array_end' symbol marks the end of the application .dtors section */
    uint32_t heap_size; 	/* This field specifies the heap size in Bytes */
    uint32_t stack_size; 	/* This field specifies the stack size required in Bytes - not used by the CRT v1.00 */
    unsigned char* entryPoint; /*This field gets populated at build time with */
    DynamicContextInstancesPtr instancesData; /*This field contains memory and shave allocation*/
    ParadigmSpecificEntry pse[TOTAL_NUM_SHAVES];
    uint32_t groupEntryPoint; /* This field holds the group entry point */
    uint64_t * appdynbssdatastart; /*This field contains a pointer to the application dynamically loadable data*/
    uint64_t * appdynbssdataend; /*This field contains the application's dynamically loadable data requirements*/
    unsigned char* appdyndata; /*This field contains a pointer to the application dynamically loadable data*/
    unsigned int appdyndatasize; /*This field contains the application's dynamically loadable data requirements*/
    void* appdyndataAllocAddr[TOTAL_NUM_SHAVES];/*This field keeps track of desired window A address to be set*/
    uint64_t * groupappdynbssdatastart;/*This field contains a pointer to the application's group dynamically loadable data*/
    uint64_t * groupappdynbssdataend;/*This field contains the group's dynamically loadable data requirements*/
    unsigned char* groupappdyndata;/*This field contains a pointer to the application's group dynamically loadable data*/
    unsigned int groupappdyndatasize;/*This field contains the group's dynamically loadable data requirements*/
    DYNCONTEXT_HEAP_ACTION_TYPE initHeap;/*This field is used to mark if the heap needs initialization or not*/
    DYNCONTEXT_APP_REENTRANT_TYPE reentrant;/*This field is used to mark if an application is reentrant or not*/
    unsigned int cmxCriticalCodeSize;/*This field specify the size of the critical code section of application*/
    void* cmxCriticalCodeAllocAddr[TOTAL_NUM_SHAVES];/*This field keeps track of desired window B address to be set*/
    void* iat;
    void* iatnames;
    void* iat_group;
    void* iatnames_group;
} DynamicContext_t;

typedef struct DynamicContextInfo_elm {
    DynamicContext_t* module;/*This field will be a pointer to a dyncontext structure*/
    char* ContextName;/*This field will hold the name of the application containing that dyncontext*/
} DynamicContextInfo_t;

typedef struct DynamicContextGlobal_elm {
    unsigned int DynamicContextAppsNumber;/*This field will contain the number of applications*/
    DynamicContextInfo_t* DynamicContextGlobalArray;/*This field points to an array with all applications*/
} DynamicContextGlobal_t;

/*This variable will be usable in order to obtain the data for all dyncontexts*/
extern DynamicContextGlobal_t GlobalContextData;

#endif
