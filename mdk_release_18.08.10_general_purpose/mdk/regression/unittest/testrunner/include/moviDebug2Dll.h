/****************************************************************************
Copyright (C) 2015 Movidius Ltd. All rights reserved
-----------------------------------------------------------------------------
File       : moviDebug2Dll.h
Description: moviDebug v2 DLL C API
Created on : Apr 20, 2015
Author     : Hanos-Puskai Péter (peter.hanos@movidius.com)
****************************************************************************/
#ifndef MOVIDEBUG_2_DLL_H_
#define MOVIDEBUG_2_DLL_H_

#include <stddef.h>
#include <stdint.h>

#define MOVIDEBUG_VERSION 0x20000000

#ifdef _MSC_VER
#define MOVIDEBUG2_CDECL __cdecl
#else
#define MOVIDEBUG2_CDECL
#endif

/* Init option keys */
#define MOVIDEBUG_OPT_TCP_HOST "tcp:host"
#define MOVIDEBUG_OPT_TCP_PORT "tcp:port"
#define MOVIDEBUG_OPT_TCP_PROXY "tcp:proxy"
#define MOVIDEBUG_OPT_CHIP_VERSION "cv"
#define MOVIDEBUG_OPT_VERBOSE "verbose"
#define MOVIDEBUG_OPT_TCF_AGENT "tcf:agent"
#define MOVIDEBUG_OPT_TCF_LISTEN_HOST "tcf:listen-host"
#define MOVIDEBUG_OPT_TCF_LISTEN_PORT "tcf:listen-port"
#define MOVIDEBUG_OPT_DDR_INIT_ELF "ddrinit:elf"
#define MOVIDEBUG_OPT_VECTOR_REGISTERS "tcf:shave-vector-registers"

/* Debugger option */
typedef struct MoviDebugDllObject* MoviDebugObjectPtr;

/* Host-specific user data passed to callbacks */
typedef void* MoviDebugUserData;

/* Log levels */
#define MOVIDEBUG_LOG_SILENT 0
#define MOVIDEBUG_LOG_DEBUG 1
#define MOVIDEBUG_LOG_VERBOSE 2
#define MOVIDEBUG_LOG_INFO 3
#define MOVIDEBUG_LOG_WARNING 4
#define MOVIDEBUG_LOG_ERROR 5

/* Callbacks provided by dll host */
typedef struct MoviDebugDllCallbacks
{
    /** requests exit from host
         can be null */
    void (MOVIDEBUG2_CDECL *exit)(MoviDebugUserData userdata, int exitcode);

    /** sends message to host
        can be null */
    void (MOVIDEBUG2_CDECL *print)(MoviDebugUserData userdata, const char *message);
    /** sends error message to host
        can be null */
    void (MOVIDEBUG2_CDECL *error)(MoviDebugUserData userdata, const char *message);

    /** sends log message to host
        can be null */
    void (MOVIDEBUG2_CDECL *log)(MoviDebugUserData userdata, int log, const char *message);

    /** allocate memory in host
        can be null if memory management is shared (same shared C/C++ runtime library) */
    void * (MOVIDEBUG2_CDECL *alloc_mem)(size_t size);
    /** free memory in host
        can be null if alloc_mem is shared */
    void (MOVIDEBUG2_CDECL *free_mem)(void *pointer);
} MoviDebugDllCallbacks;

/** Dll initialisation option structure */
typedef struct MoviDebugInitOption
{
    /** fill with MOVIDEBUG_OPT_<...>  */
    const char *key;
    const char *value;
} MoviDebugInitOption;

typedef struct MoviDebugDllInitOptions
{
    /** fill with MOVIDEBUG_VERSION */
    int version;
    /** arbitrary user data passed to host callbacks */
    MoviDebugUserData userdata;
    /** count of init options */
    int optionCount;
    /** init options */
    const MoviDebugInitOption *initOptions;
    /** callback interface */
    const MoviDebugDllCallbacks *callbacks;
} MoviDebugDllInitOptions;

typedef uint32_t MoviDebugDllAddress;
typedef uint32_t MoviDebugDllSize;
typedef uint32_t MoviDebugDllWord32;

/* valid values for locType argument */
#define MOVIDEBUG_DLL_LOCTYPE_ANY  ((char*) 0)
#define MOVIDEBUG_DLL_LOCTYPE_ADDR "-addr"
#define MOVIDEBUG_DLL_LOCTYPE_REGISTER "-reg"
#define MOVIDEBUG_DLL_LOCTYPE_VARIABLE "-var"
#define MOVIDEBUG_DLL_LOCTYPE_SYMBOL "-sym"
#define MOVIDEBUG_DLL_LOCTYPE_EXPRESSION "-expr"

#define MOVIDEBUG_DLL_DEFAULT_TARGET ((char*) 0)

/* NOT IMPLEMENTED
   SUBJECT TO CHANGE
   called on TCF dispatch thread */
typedef void (MOVIDEBUG2_CDECL *MoviDebugTcfResponseCallback)(void *userdata, char responseType, const char* responseData, size_t responseSize);

/** Movidebug2 API
    Binary interface returned by init function
    Client can assume all none-reserved members to be non-null */
typedef struct MoviDebugDllInterface
{
    /** filled with size of the structure */
    size_t size;
    /** points to static memory */
    const char *versionString;
    /** NULL */
    void *reserved0;
    /** NULL */
    void *reserved1;

    /**
       Evaluate Tcl script

       result may be NULL if not required
       non-null input `*result` is freed using `MoviDebugDllCallbacks.free_mem` before setting new value
       new value in `*result` is allocated using `MoviDebugDllCallbacks.alloc_mem``
       result is always null-terminated but may contain inner null characters (e.g binary strings)
       return value is zero on success
     */
    int (MOVIDEBUG2_CDECL *tclEvalScript)(MoviDebugObjectPtr dbg, const char *script, char **result, size_t *resultSize);

    /**
       Evaluate Tcl command.

       This is better suited to avoid manual Tcl escapes when the case.
       `argc` may be negative, in this case `argv` must be null-terminated.
       `result` may be NULL if not required;
       non-null input `*result` is freed using `MoviDebugDllCallbacks.free_mem` before setting new value
       new value in `*result` is allocated using `MoviDebugDllCallbacks.alloc_mem``
       result is always null-terminated but may contain inner null characters (e.g. binary strings)
       return value is zero on success
     */
    int (MOVIDEBUG2_CDECL *tclEvalCommand)(MoviDebugObjectPtr dbg, int argc, const char * const *argv, char **result, size_t *resultSize);

    /** send TCF command
        NOT IMPLEMENTED
        callback function will be executed on the TCF event dispatch thread
     */
    int (MOVIDEBUG2_CDECL *sendTcfCommand)(MoviDebugObjectPtr dbg, const char *service, const char *command, void *data, size_t size, MoviDebugTcfResponseCallback callback, void *userData);

    /** set value of tcl variable */
    int (MOVIDEBUG2_CDECL *setTclVar)(MoviDebugObjectPtr dbg, const char *name, const char *value);
    /** get value of tcl variable */
    int (MOVIDEBUG2_CDECL *getTclVar)(MoviDebugObjectPtr dbg, const char *name, char *result, size_t maxSize);

    /** raw register read using the movidebug protocol */
    int (MOVIDEBUG2_CDECL *read32)(MoviDebugObjectPtr dbg, MoviDebugDllAddress address, MoviDebugDllWord32 *result);
    /** raw register write using the protocol */
    int (MOVIDEBUG2_CDECL *write32)(MoviDebugObjectPtr dbg, MoviDebugDllAddress address, MoviDebugDllWord32 value);

    /** raw block read using the protocol */
    int (MOVIDEBUG2_CDECL *readBlock)(MoviDebugObjectPtr dbg, MoviDebugDllAddress address, void *result, MoviDebugDllSize size);
    /** raw block write using the protocol */
    int (MOVIDEBUG2_CDECL *writeBlock)(MoviDebugObjectPtr dbg, MoviDebugDllAddress address, const void *data, MoviDebugDllSize size);

    /** load binary file to address, register or symbol */
    int (MOVIDEBUG2_CDECL *loadBinFile)(MoviDebugObjectPtr dbg, const char* location, const char *fileName);
    /** save binary file from address, register or symbol */
    int (MOVIDEBUG2_CDECL *saveBinFile)(MoviDebugObjectPtr dbg, const char* location, MoviDebugDllSize size, const char *fileName);

    /** load executable elf (`mdbg::loadfile -elf`)
         startupCore is optional (calls `mdbg::startupcore`) */
    int (MOVIDEBUG2_CDECL *loadElf)(MoviDebugObjectPtr dbg, const char *fileName, const char *startupCore);

    /** get binary data of arbitrary size similar to `mdbg::mget` Tcl command */
    int (MOVIDEBUG2_CDECL *mget)(MoviDebugObjectPtr dbg, const char *target, const char *loctype, const char *location, void *result, MoviDebugDllSize size);
    /** set binary data of arbitrary size similar to `mdbg::mset` Tcl command */
    int (MOVIDEBUG2_CDECL *mset)(MoviDebugObjectPtr dbg, const char *target, const char *loctype, const char *location, const void *value, MoviDebugDllSize size);

    /** get address of symbol (Tcl command `mdbg::sym addr`) */
    int (MOVIDEBUG2_CDECL *symAddr)(MoviDebugObjectPtr dbg, const char *target, const char *name, MoviDebugDllAddress *result);
    /** get name of symbol at address (Tcl command: `mdbg::sym at -q -first`) */
    int (MOVIDEBUG2_CDECL *symName)(MoviDebugObjectPtr dbg, const char *target, MoviDebugDllAddress address, char *result, size_t maxSize);

    /** get address of register (similar to `mdbg::getRegAddr` Tcl command,
      but also recognizes some target-specific register names like `i0`)
      (similar to mdgb::mget -reg, but value is not retrieved)
     */
    int (MOVIDEBUG2_CDECL *regAddr)(MoviDebugObjectPtr dbg, const char *target, const char *name, MoviDebugDllAddress *result);
} MoviDebugDllFunctions;

/* Define this if you don't link implicitly to the DLL/SO (e.g moviDebug2 shell) */
#ifdef MOVIDEBUG_2_DLL_EXPLICIT_LINK

/* function names */
#define MOVIDEBUG_DLL_INIT_FUN_NAME "moviDebug2_DllInit"
#define MOVIDEBUG_DLL_DONE_FUN_NAME "moviDebug2_DllDone"
#define MOVIDEBUG_DLL_COMMAND_FUN_NAME "moviDebug2_DllCommand"

/* function pointer types */
typedef MoviDebugObjectPtr (MOVIDEBUG2_CDECL *MoviDebugDllInitFun)(const MoviDebugDllInitOptions *initOptions, const MoviDebugDllInterface* *dllInterfacePtr);
typedef int (MOVIDEBUG2_CDECL *MoviDebugDllCommandFun)(MoviDebugObjectPtr dbg, const char *command, char **result);
typedef void (MOVIDEBUG2_CDECL *MoviDebugDllDoneFun)(MoviDebugObjectPtr dbg);

#else

#   ifdef MOVIDEBUG_2_INTERNAL_BUILD
#       include "dllimpex.h"
#       define MOVIDEBUG_DLLAPI DLL_EXPORT
#   else
#       ifdef _WIN32
#          define MOVIDEBUG_DLLAPI __declspec(dllimport)
#       else
#          if __GNUC__ >= 4
#              define MOVIDEBUG_DLLAPI __attribute__ ((visibility ("default")))
#          else
#              define MOVIDEBUG_DLLAPI
#          endif
#       endif
#   endif

#   ifdef __cplusplus
extern "C" {
#   endif

/**
 * Initialisation function.
 * \param initOptions configuration structure
 * \param[out,optional] dllInterfacePtr: receives pointer to dll interface
 * \return Debugger object created or `NULL` on failure
 * \note:
 *  - Tcl subsystem (including commands) will only be initialised if `dllInterfacePtr` is not null.
 *  - procedure will call `initOptions->callbacks->exit(errorcode)` on failure to initialize
 *  -
 */
MOVIDEBUG_DLLAPI
MoviDebugObjectPtr
MOVIDEBUG2_CDECL
moviDebug2_DllInit(const MoviDebugDllInitOptions *initOptions, const MoviDebugDllInterface* *dllInterfacePtr);

/** Finalizer
 * \param dbg Debugger object created by moviDebug2_DllInit
 */
MOVIDEBUG_DLLAPI
void
MOVIDEBUG2_CDECL
moviDebug2_DllDone(MoviDebugObjectPtr dbg);


MOVIDEBUG_DLLAPI
int
MOVIDEBUG2_CDECL
moviDebug2_DllCommand(MoviDebugObjectPtr dbg, const char *command, char **result);

#   ifdef __cplusplus
 } // extern "C"
#   endif

#endif /*ndef MOVIDEBUG_2_DLL_EXPLICIT_LINK */

#endif /* MOVIDEBUG_2_DLL_H_ */
