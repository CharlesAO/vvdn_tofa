#ifndef _LIB_FPROFILE_H_
#define _LIB_FPROFILE_H_

#include "ptypes.h"

#include <stdio.h>

#ifdef __RTEMS__
// RTEMS includes
#include <rtems.h>
#include <rtems/score/sysstate.h>
#include <rtems/score/object.h>
#include <rtems/libio.h> // rtems_libio_ioctl_args_t
/* #include <OsDrvResMgr.h> // OS_DRV_RESMGR_EXECUTE_PROTECTED */
/* #include <OsDrvMutex.h> */
#elif !defined(__shave__)
// BM includes
#include <swcLeonUtils.h>
#include <DrvMutex.h>
/* #include <DrvResMgr.h> SglResMgrClk - not available for MX, do fwd decl of
 * function */
#else
// SHAVE includes
#include <ShDrvMutex.h> // ShDrvMutexRequest
#endif

#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <swcWhoAmI.h>
#ifdef __RTEMS__
#include <OsDrvCpr.h>
#else
#include <DrvCpr.h>
#endif // __RTEMS__

#include <ShaveData.h>

#define MV_PROF_VERSION         (6u)

/**
 * @brief config options
 *
 * CONFIG_STATIC_LIB_PROFILER - define when a static lib is built
 * CONFIG_INLINE_PROFILER - define when all functions are
 * static inline, __cyg symbols will dissapear. Faster but biggerr
 * __RTEMS__ (auto defined)
 *
 */

// revisit the following TODO's at the end
// TODO: verify all #if have known defines
// TODO: every weak symbol should have __ prefix
// TODO: use compiler defined __ma2100__ __ma2150__ __ma2155__ __ma2450__ __ma2455__ __ma2080__ __ma2085__ __ma2480__ __ma2485__
// TODO: sync with initial code
// TODO: make (almost) all internal functions (PROF_API) static inline pure const
// TODO: check with nm what symbols are still exported from profiling


// workaround section
// workaround section end

/**
 * Decorators:
 * -----------
 *
 * PROF_API             - private function
 * PUBLIC_PROF_API      - public function
 * PROF_INIT_FUNCTION   - constructor (init function)
 *
 */
//#define PROF_API __attribute__((no_instrument_function, nothrow, pure, const)) static inline - when pure functional (no static var, no globals)
#ifdef PROFILE_DEBUG
#define PROF_API __attribute__((no_instrument_function, nothrow))
#else
#define PROF_API __attribute__((no_instrument_function, nothrow)) static inline
#endif

#ifdef CONFIG_STATIC_LIB_PROFILER
#define PUBLIC_PROF_API __attribute__((no_instrument_function, nothrow))
#define PROF_INIT_FUNCTION PUBLIC_PROF_API __attribute__((constructor))
#else
// header based only project. Use this *ONLY* if you need extra speed and have enough memory

#ifdef CONFIG_INLINE_PROFILER
// header-only everything inlined
#define PUBLIC_PROF_API PROF_API
#else
// header-only file
// you are not weak just because your heart feels so heavy
#define PUBLIC_PROF_API __attribute__((weak, no_instrument_function, nothrow))
#endif

#ifdef EXPLICIT_INIT_FUNCTION
#define PROF_INIT_FUNCTION __attribute__((weak, no_instrument_function, nothrow))
#else
#define PROF_INIT_FUNCTION __attribute__((weak, constructor, no_instrument_function, nothrow))
#endif // EXPLICIT_INIT_FUNCTION

#endif // integration type CONFIG_STATIC_LIB_PROFILER, CONFIG_INLINE_PROFILER, or CONFIG_INLINE...

#ifndef PROFILER_MUTEX
#define PROFILER_MUTEX (5) /* TODO */
#endif

#if defined(MA2X8X) && defined(__RTEMS__)
#include <OsDrvMutex.h>
extern OsDrvMutexHandler _p_hndl;
#endif

#ifdef MA2X5X
#define PRF_OS_LOCK() {                            \
        osDrvMutexHandle_t handle;                 \
        OsDrvMutexOpen(&handle, PROFILER_MUTEX);   \
        OsDrvMutexObtain(&handle);
#define PRF_OS_UNLOCK()                            \
        OsDrvMutexRelease(&handle);                \
        OsDrvMutexClose(&handle);                  \
    }
#elif defined(MA2X8X)
#define PRF_OS_LOCK() {                            \
        /*OsDrvMutexHandler hndl; */                   \
        if (!_p_hndl.id) { OsDrvMutexOpen(&_p_hndl, PROFILER_MUTEX); }     \
        OsDrvMutexObtain(&_p_hndl);
#define PRF_OS_UNLOCK()                            \
        OsDrvMutexRelease(&_p_hndl);                  \
        /*OsDrvMutexClose(&hndl);*/                    \
    }
#else
#error "unknown platform!rm"
#endif

#ifdef __leon__
#ifdef __RTEMS__
#define CRITICAL_SECTION_ENTER() do { \
    PRF_OS_LOCK();
#else
#define CRITICAL_SECTION_ENTER() do { \
    DrvMutexLock(PROFILER_MUTEX);
#endif

#ifdef __RTEMS__
#define CRITICAL_SECTION_EXIT() \
    PRF_OS_UNLOCK(); \
    } while (0);
#else
#define CRITICAL_SECTION_EXIT() \
    DrvMutexUnlock(PROFILER_MUTEX); \
    } while (0);
#endif
#else // shave
#define CRITICAL_SECTION_ENTER() do { \
    ShDrvMutexRequest(PROFILER_MUTEX); \
    asm volatile ("":::"cc", "memory")
#define CRITICAL_SECTION_EXIT() \
    asm volatile ("":::"cc", "memory");\
    ShDrvMutexRelease(PROFILER_MUTEX); \
    } while (0)
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// profiler enable disable    ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum ProfilerState_t {
    PROF_READ_VALUE = -1,
    PROF_DISABLED = 0,
    PROF_ENABLED = 1
} ProfilerState_t;

PROF_API
int profilerState(const ProfilerState_t state) {
    static int _profilerEnabled = 0;
    if (state == PROF_DISABLED || state == PROF_ENABLED) {
        _profilerEnabled = state;
    }

    return _profilerEnabled;
}

PUBLIC_PROF_API
void enableProfiler() {
    profilerState(PROF_ENABLED);
}
PUBLIC_PROF_API
void disableProfiler() {
    profilerState(PROF_DISABLED);
}
PUBLIC_PROF_API
__attribute__((warn_unused_result))
int profilerEnabled() {
    return profilerState(PROF_READ_VALUE);
}



#define unlikely(x) __builtin_expect(!!(x), 0)

PROF_API void __allocateNewChunk();

#if defined(CONFIG_PRECOMPILED_LIB_PROFILER)
#error Why this is included? MUST use the precompiled .a instead
#endif

// TODO: rework this when moviCompile will have weak
// http://dub30.ir.intel.com/bugzilla/show_bug.cgi?id=28612
//static  ProfileBufferEntry_t* entry = NULL;
//#define SET_ENTRY(value) entry = value
//#define ENTRY() entry
PROF_API ProfileBufferEntry_t** ENTRY() {
    static ProfileBufferEntry_t* _entry = NULL;
    return &_entry;
}
PROF_API void SET_ENTRY(ProfileBufferEntry_t* value) {
    *ENTRY() = value;
}

PROF_API
void profilePushEntry(uint32_t address, uint32_t ctx, uint64_t ts) {
    ProfileBufferEntry_t * __restrict__ entry = *ENTRY();
    if (!entry) return;
    p_assert(entry->head);
    entry->head->functionAddress = address;
    entry->head->context = ctx;
    entry->head->timestamp = ts;
    entry->head++;
    if (unlikely(entry->head == entry->end)) {
        __allocateNewChunk();
    } else {
        p_assert(entry->head < entry->end);
    }
}

///////////////////////////////////////////////////////////////////
// Timestamp Reading
///////////////////////////////////////////////////////////////////
//
#ifndef FP_TIMER_ADDR
#define FP_TIMER_ADDR TIM0_BASE_ADR
#endif

#ifdef FAST_FP_TIME_READ
#ifdef MA2X8X
#define FP_TIME_READ() ({ \
    uint64_t ___low  = (uint64_t)GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT0_OFFSET); \
    uint64_t ___high  = (uint64_t)GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT1_OFFSET); \
    u64 ___full = (___high  << 32) | ___low; \
    ___full; \
})
#else
#define FP_TIME_READ() ({ \
    uint64_t ___high  = (uint64_t)GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT1_OFFSET); \
    uint64_t ___low  = (uint64_t)GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT0_OFFSET); \
    uint64_t ___full = (___high  << 32) | ___low; \
    ___full; \
})
#endif

#else

PROF_API uint64_t _getTime() {
#ifdef MA2X8X
    uint64_t ___low  = (uint64_t)GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT0_OFFSET);
    uint64_t ___high  = (uint64_t)GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT1_OFFSET);
#else
    uint64_t ___high = (uint64_t) GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT1_OFFSET);
    uint64_t ___low = (uint64_t) GET_REG_WORD_VAL(FP_TIMER_ADDR + TIM_FREE_CNT0_OFFSET);
#endif
    return (___high << 32) | ___low;
}

#define FP_TIME_READ() _getTime()
#endif // FAST_FP_TIME_READ

///////////////////////////////////////////////////////////////////
// overhead computation
///////////////////////////////////////////////////////////////////

// measure overhead clocks. this can be influenced by dynamic frequency scaling (if the case)
static inline void
__instrumentedFunction(void) {
    noopt();
}
static inline void
__attribute__((no_instrument_function))
__notInstrumentedFunction(void) {
    noopt();
}

/**
 * Self measure hooks overhead
 */
PROF_API
uint32_t
__computeOverhead() {
    volatile uint64_t t1, t2, t3;
#ifdef __RTEMS__
    System_state_Codes prevState = _System_state_Current;
    _System_state_Current = SYSTEM_STATE_UP;
#endif
    int prfEnabled = profilerEnabled();
    if (!prfEnabled) enableProfiler();
    noopt();
    t1 = FP_TIME_READ();
    noopt();
    __instrumentedFunction();
    noopt();
    t2 = FP_TIME_READ();
    noopt();
    __notInstrumentedFunction();
    noopt();
    t3 = FP_TIME_READ();
    noopt();
    if (!prfEnabled) disableProfiler(); // restore profiler flag

    uint32_t overhead = 0;

    if (t1<t2 && t2<t3) {
        uint64_t instrumentedTime = t2 - t1;
        uint64_t notInstrumentedTime = t3 - t2;
        overhead = (uint32_t) (instrumentedTime - notInstrumentedTime);
    } else {
        overhead = 0xFFFFFFFF;
    }

#ifdef __RTEMS__
    _System_state_Current = prevState;
#endif
    return overhead;
}

/*
======================================
ProfileEntry_t structure memory layout
======================================

+===========+=========+===========+
| addr_func | context | timestamp |
+===========+=========+===========+
| 32 bit    | 32 bit  | 64 bit    |
+-----------+---------+-----------+


context field (32 bit)
+============+===========+===========+============+
| Thread API | Thread ID | Reserved  | Entry Type |
+============+===========+===========+============+
|   31 .. 29 |  28 .. 25 |  24 .. 4  |     3 .. 0 |
+------------+-----------+-----------+------------+
If is ISR context, then Thread ID is the interrupt
source (ICB.SRC) and thread API is 111 (0x7)
*/


/*
no need to send the tid for every function entry/exit since the CTX_SWITCH will do the job
just fine
f_addr 32 bit aligned (lat 2 bits always 0 - use the location to save type)
f entry: f_addr | 0x1, timestamp
f exit : f_addr | 0x2, timestamp
ctx sw : (old/new ctx) << 2 | 0x3, timestamp
memory : (addr & ~0x3), timestamp
*/
// Since one can use more API in one app, TID contains also API: 15:13 is API while 12:0 is index
// thread switch or irq enter/irq exit

extern const int __buffer_size__;
extern char __profile_internal_buffer__[];
#define CORE_BUFFER_SIZE (__buffer_size__ / (TOTAL_NUM_SHAVES + 2))

/**
 * Buffer structure:
 * ╔═══════════════════════════╦═════════╦═════════╦═════╦═════════╗
 * ║ ProfileBufferMasterRecord ║ CHUNK 1 ║ CHUNK 2 ║ ... ║ CHUNK n ║
 * ╚═══════════════════════════╩═════════╩═════════╩═════╩═════════╝
 *
 * ProfileBufferMasterRecord:
 * ╔═══════╦═══════╦═════════╦═══════════════╦═══════════════════╦══════════╦═════════════╗
 * ║       ║ magic ║ version ║ profileBuffer ║ myriadChipVersion ║ entryCnt ║ _unused[12] ║
 * ║ bytes ║ 4     ║ 4       ║ 4             ║ 4                 ║ 4        ║ 12          ║
 * ╚═══════╩═══════╩═════════╩═══════════════╩═══════════════════╩══════════╩═════════════╝
 *
 * CHUNK:
 * ╔════════════════════╦══════════════╦══════════════╦═════╦══════════════╦═══╦═══╦═════╦═══╗
 * ║ ProfileBufferEntry ║ ProfileEntry ║ ProfileEntry ║ ... ║ ProfileEntry ║ 0 ║ 0 ║ ... ║ 0 ║
 * ║      32 bytes      ║   16 bytes   ║              ║     ║              ║   ║   ║     ║   ║
 * ╚════════════════════╩══════════════╩══════════════╩═════╩══════════════╩═══╩═══╩═════╩═══╝
 *
 */


PROF_API
ProfileBufferMasterRecord_t* _getMasterRecord() {
    int offset = CORE_BUFFER_SIZE * (swcWhoAmI() - 1);
    return (ProfileBufferMasterRecord_t*) (__profile_internal_buffer__ + offset);
}

PROF_API
void _initProfileIndex(ProfileBufferEntry_t* fProfile) {
    fProfile->head = (ProfileEntry_t *) &fProfile[1];
    fProfile->end = &fProfile->head[((FPROFILER_CHUNK_SIZE - sizeof(ProfileBufferEntry_t)) / sizeof(ProfileEntry_t))];
    if (fProfile->end > (ProfileEntry_t*)(__profile_internal_buffer__ + __buffer_size__)) {
        fProfile->head = 0; // END
        return;
    }
    fProfile->headerSize = sizeof(ProfileBufferEntry_t);
    fProfile->startTime = FP_TIME_READ();
    fProfile->coreId = swcWhoAmI();
    fProfile->bufferType ='F';
    fProfile->overhead = __computeOverhead();
    __builtin_memset(fProfile->head, 0, FPROFILER_CHUNK_SIZE - sizeof(ProfileBufferEntry_t));
}

#define _SFY(x) #x
#define SFY(x) _SFY(x)

PROF_API void __profilerEnd() {
    disableProfiler();
#ifndef __shave__
    int32_t freq = 0;
    volatile ProfileBufferMasterRecord_t* mr = _getMasterRecord();
#ifdef __RTEMS__
 #if defined(__ma2480__) || defined(__ma2485__) || defined(MA2X8X)
    OsDrvCprGetClockFrequency(0,(uint32_t*)&freq);
 #elif defined(__ma2150__) || defined(__ma2450__) || defined(__ma2455__) || defined(MA2X5X)
    OsDrvCprGetClockFreqKhz(SYS_CLK, NULL, &freq);
 #else
 #error "undefined platform"
 #endif
#else // __RTEMS__
    freq = DrvCprGetClockFreqKhz(SYS_CLK, NULL);
#endif // __RTEMS__
    if ( freq )
        mr->sysClockMhz = freq;
#endif // if __shave__ code
}

PROF_API void __initMasterRecord(ProfileBufferMasterRecord_t* mr) {
    // first byte after the master record is the first ProfileBufferEntry
    ProfileBufferEntry_t* pProfileBufferEntry = (ProfileBufferEntry_t *) &mr[1];
    mr->magic = 0x666f7270;
    mr->version = MV_PROF_VERSION;
    mr->myriadChipVersion =
#if defined(MA2450)
                "MA2450";
#elif defined(MA2150)
                "MA2150";
#elif defined(MA2155)
                "MA2155";
#elif defined(MA2455)
                "MA2455";
#elif defined(MA2480)
                "MA2480";
#else
                "unknown";
#endif
    mr->toolsVersion = SFY(__MOVITOOLS__);
    mr->entryCnt = 1;
    _initProfileIndex(pProfileBufferEntry);
    SET_ENTRY(pProfileBufferEntry);
}

PROF_API
void __allocateNewChunk() {
    ProfileBufferMasterRecord_t* mr = _getMasterRecord();
    // CRITICAL SECTION ENTER
    ProfileBufferEntry_t* pProfileBufferEntry = (ProfileBufferEntry_t*) (
            ((char*)mr)
                + sizeof(ProfileBufferMasterRecord_t)
                + (mr->entryCnt * FPROFILER_CHUNK_SIZE)
            );
    mr->entryCnt++;
    // CRITICAL SECTION EXIT
    if ( (void*)pProfileBufferEntry - (void*)mr > CORE_BUFFER_SIZE ) {
        // broken promises
        // check policy
        pProfileBufferEntry = NULL;
    } else {
        _initProfileIndex(pProfileBufferEntry);
    }
    SET_ENTRY(pProfileBufferEntry);
}

PROF_API
void
__functionProfilerInit() {
    // FIXME: I hope LOS is the first core started, if not I'll have to figure out something else
    uint8_t coreId = swcWhoAmI();
    if (coreId == 1) {
        __builtin_memset(__profile_internal_buffer__, 0, __buffer_size__);
    }
    ProfileBufferMasterRecord_t* mr = _getMasterRecord();
    if (mr->magic == 0x666f7270 && mr->version == MV_PROF_VERSION) {
        // master record already initialized
        if (coreId < 3) {
            // leon restarted, ddr not cleared
            p_assert(coreId == 2); // otherwise memset is broken
        } else {
            // TODO:
        }
        __allocateNewChunk();
    } else {
        // init MR here
        __initMasterRecord(mr);

        int atexit(void (*function)(void));
        atexit(__profilerEnd);
    }
}


#if defined(__sparc__)
#ifndef __RTEMS__

PROF_API uint32_t patchTrapHandlerForIrqProfiling();

PUBLIC_PROF_API
void
__attribute__((nothrow))
__attribute__((optimize(3)))
__cyg_profile_func_enter (void *func,  void * caller __attribute__((unused)))
{
    uint32_t pil;
    if (!profilerEnabled()) return;
//    disableProfiler();
    pil = (uint32_t) swcLeonSetPIL(15);

    profilePushEntry((uint32_t)(func), HOOK_FUNC_ENTRY, FP_TIME_READ());
#ifdef FP_LEON_FCT_ENTER_HOOK
    FP_LEON_FCT_ENTER_HOOK
#endif

    swcLeonSetPIL(pil);
//    enableProfiler();
}

PUBLIC_PROF_API
void
__attribute__((nothrow))
__attribute__((optimize(3)))
__cyg_profile_func_exit (void *func, void *caller __attribute__((unused)))
{
    uint32_t pil;
    if (!profilerEnabled()) return;
//    disableProfiler();
    pil = (uint32_t) swcLeonSetPIL(15);

#ifdef FP_LEON_FCT_EXIT_HOOK
    FP_LEON_FCT_EXIT_HOOK
#endif
    profilePushEntry((uint32_t)(func), HOOK_FUNC_EXIT, FP_TIME_READ());

    swcLeonSetPIL(pil);
//    enableProfiler();
}

__attribute__((weak)) uint32_t irqSource = 0;
__attribute__((weak)) uint32_t nesting = 0;

PROF_API
void
__attribute__((optimize(3)))
hookIrqProfileFn( int irq_src )
{
    extern void (*__irq_table[])( int );
    void (*irq_handler)(int) = __irq_table[ irq_src ];
    uint32_t pil;

    if (!profilerEnabled()) {
        (*irq_handler)( irq_src );
        return;
    }

    pil = (uint32_t) swcLeonSetPIL(15);
    // 0x11 .. 0x1E (0x1F - reserved)
    uint32_t newIrqSource = (uint32_t) ((irq_src & 0xF) | 0xE000);
    uint32_t oldIrqSource = irqSource;
    irqSource = newIrqSource;
    nesting++;
    profilePushEntry(oldIrqSource, (newIrqSource << 16) | HOOK_TASK_SWITCH, FP_TIME_READ());
    swcLeonSetPIL(pil);


    // actually call the handler
    (*irq_handler)( irq_src );

    swcLeonSetPIL(15);
    if (--nesting == 0)
        irqSource = 0;
    profilePushEntry(newIrqSource, (oldIrqSource << 16) | HOOK_TASK_SWITCH, FP_TIME_READ());
    swcLeonSetPIL(pil);
}

// After an idea from Mircea Horia Ionica
// encoding of the instruction we need to patch
// call %l6, aka jmpl %l6+%g0, %o7
#define IRQ_CALL_INSTR 0x9fc58000
// return the address of the instruction that was patched, 0 on error
PROF_API
uint32_t
__attribute__((optimize(3)))
patchTrapHandlerForIrqProfiling()
{
  extern uint32_t traphandler[];  // the actual code of the trap handler
  uint32_t patch_addr = 0;
  // scan the trap handler code (but no more than 6K) for the instruction to be patched
  unsigned call_ofs;
  for( call_ofs = 0; call_ofs < 6*1024/sizeof(uint32_t); ++call_ofs )
    if( traphandler[ call_ofs ] == IRQ_CALL_INSTR ) {
      patch_addr = (uint32_t)&traphandler[ call_ofs ];
      break;
    }
  // fail if we couldn't find the instruction
  if( !patch_addr )
    return 0;
  // build the new instruction - call hookIrqProfileFn
  // note this depends on both the call's address and the target address
  // instruction bitmap: (call imm30) [01iiiiiiiiiiiiiiiiiiiiiiiiiiiiii]
  // effect: %o7 <= %pc, %npc <= %pc + (imm30<<2)
  const uint32_t hook_addr = (uint32_t)&hookIrqProfileFn;
  const uint32_t displacement = hook_addr - patch_addr;
  const uint32_t new_opcode =( 0x1 << 30 )|( displacement >> 2 );
  // actually patch the trap handler
  traphandler[ call_ofs ] = new_opcode;

  // make sure the change is picked up by L1I
  swcLeonInstructionCacheFlush();

  // return success
  return patch_addr;
}



//uint32_t fpInitCode = 0;

PROF_INIT_FUNCTION
void __leonProfilerInit(void) {
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;
    __functionProfilerInit();
    (void) patchTrapHandlerForIrqProfiling(); // TODO: check result
    enableProfiler();
}

#else // have RTEMS

// TODO: potential problem:
// for nested interrupts, in irq1 is raised irq2, the ICB.SRC is updated, and
// then after irq2 handler finishes then returning into irq1, the getInterruptSource()
// will be false. add an array of int representing the level - increment on entry, decrement on exit

PROF_API
uint32_t getInterruptSource()
{
    if (rtems_interrupt_is_in_progress()) {
        return (((swcWhoAmI() == 1) ? (
            GET_REG_WORD_VAL(ICB0_SRC_ADR) ) :
               ( GET_REG_WORD_VAL(ICB1_SRC_ADR) ))
            & 0xF ) | 0x07000000; // use thread API=111 to signal this is an irq
    }

    return 0;
}

// TODO: use __attribute__ ((ifunc ("profiler_entry_exit_etc"))) to add the init stuff

__attribute__((weak)) uint32_t lastIrqSource = 0;

#define SPARC_SWTRAP_IRQDIS 9
#define SPARC_SWTRAP_IRQEN 10

__attribute__((no_instrument_function))
static inline uint32_t _sparc_disable_interrupts(void)
{
  register uint32_t psr __asm__("g1"); /* return value of trap handler */
  __asm__ volatile ( "ta %1\n\t" : "=r" (psr) : "i" (SPARC_SWTRAP_IRQDIS));
  return psr;
}

__attribute__((no_instrument_function))
static inline void _sparc_enable_interrupts(uint32_t psr)
{
  register uint32_t _psr __asm__("g1") = psr; /* input to trap handler */

  /*
   * The trap instruction has a higher trap priority than the interrupts
   * according to "The SPARC Architecture Manual: Version 8", Table 7-1
   * "Exception and Interrupt Request Priority and tt Values".  Add a nop to
   * prevent a trap instruction right after the interrupt enable trap.
   */
  __asm__ volatile ( "ta %0\nnop\n" :: "i" (SPARC_SWTRAP_IRQEN), "r" (_psr));
}

/// @brief Enter hook function, instrumentation call for entry of the fucntion
/// @param[in] func          - address of the start of the current function
/// @param[in] caller        - address of the function's call site
/// @attribute no_instrument_function - tells to compiler - don't instrument this function calls
/// for this function
/// @attribute optimize(3) - try to max optimize the code no matter the global options
/// @return void
///
PUBLIC_PROF_API
void
__attribute__((optimize(3)))
__cyg_profile_func_enter (void *func, void *caller __attribute__((unused)))
{
    rtems_interrupt_level interrupts  = 0;

    /* do not monitor if profiler not enabled or system not yet started */
    if ( !profilerEnabled() || _System_state_Current != SYSTEM_STATE_UP )
        return;

    // TODO: replace with rtems_interrupt_local_disable() when bugzilla ticket
    // 29505 is fixed
    interrupts = _sparc_disable_interrupts();
    /* disableProfiler(); */
    /* rtems_interrupt_disable(interrupts); */

    rtems_id tid = rtems_task_self();
    uint64_t now = FP_TIME_READ();

    if (rtems_interrupt_is_in_progress()) {
        // in this case tid is the ID of interrupted task
        uint32_t currentIrqSource = getInterruptSource();
        if (currentIrqSource != lastIrqSource) {
            // send a special thread switch event
            if (lastIrqSource == 0) {
                profilePushEntry((((((tid)&0x07000000)>>11)&0xe000) | ((tid)&0x1fff)) & 0xffff,
                             (((currentIrqSource)&0x07000000)<<5) | (((currentIrqSource)&0x1FFF)<<16) | HOOK_TASK_SWITCH, now-2);
            } else {
                profilePushEntry((((((lastIrqSource)&0x07000000)>>11)&0xe000) | ((lastIrqSource)&0x1fff)) & 0xffff,
                             (((currentIrqSource)&0x07000000)<<5) | (((currentIrqSource)&0x1FFF)<<16) | HOOK_TASK_SWITCH, now-3);
            }
            lastIrqSource = currentIrqSource;
        } else {
            // thread switch event was already sent
        }
        tid = currentIrqSource;
    }
    else {
        if (lastIrqSource) {
            // we left an ISR. make a switch
            profilePushEntry((((((lastIrqSource)&0x07000000)>>11)&0xe000) | ((lastIrqSource)&0x1fff)) & 0xffff,
                    (((tid)&0x07000000)<<5) | (((tid)&0x1FFF)<<16) | HOOK_TASK_SWITCH, now-4);
            lastIrqSource = 0;
        }
    }

    // tid is 32 bit value. We are interested in API26:24 and index 15:0
    profilePushEntry((uint32_t)(func), (((tid)&0x07000000)<<5) | (((tid)&0x1FFF)<<16) | HOOK_FUNC_ENTRY, now);
    // call UDF hook
#ifdef FP_RTEMS_FCT_ENTER_HOOK
    FP_RTEMS_FCT_ENTER_HOOK
#endif
    /* enableProfiler(); */
    _sparc_enable_interrupts(interrupts);
    /* rtems_interrupt_enable(interrupts); */
}

/// @brief Exit hook function, instrumentation call for exit of the fucntion
/// @param[in] func          - address of the start of the current function
/// @param[in] caller        - address of the function's call site
/// @attribute no_instrument_function - tells to compiler - don't instrumentation call
/// for this function
/// @attribute optimize(3) - try to max optimize the code no matter the global options
/// @return void
PUBLIC_PROF_API
void
__attribute__((optimize(3)))
__cyg_profile_func_exit (void *func, void *caller __attribute__((unused)))
{
    rtems_interrupt_level interrupts = 0;
    rtems_id tid;

    /* do not monitor if profiler not enabled or system not yet started */
    if (  !profilerEnabled() || _System_state_Current != SYSTEM_STATE_UP )
        return;

    interrupts = _sparc_disable_interrupts();
    /* disableProfiler(); */
    /* rtems_interrupt_disable(interrupts); */

    // call UDF hook
#ifdef FP_RTEMS_FCT_EXIT_HOOK
    FP_RTEMS_FCT_EXIT_HOOK
#endif
    tid = rtems_task_self();
    uint64_t now = FP_TIME_READ();
    if (lastIrqSource) {
        if (rtems_interrupt_is_in_progress()){
            tid = lastIrqSource;
        } else {
            profilePushEntry((((((lastIrqSource)&0x07000000)>>11)&0xe000) | ((lastIrqSource)&0x1fff)) & 0xffff,
                    (((tid)&0x07000000)<<5) | (((tid)&0x1FFF)<<16) | HOOK_TASK_SWITCH, now-5);
            lastIrqSource = 0;
        }
    }
    profilePushEntry((uint32_t)(func), (((tid)&0x07000000)<<5) | (((tid)&0x1FFF)<<16) | HOOK_FUNC_EXIT, now);

    /* enableProfiler(); */
    _sparc_enable_interrupts(interrupts);
    /* rtems_interrupt_enable(interrupts); */
}

PUBLIC_PROF_API
__attribute__((optimize(3)))
void thread_switch_extension(Thread_Control *executing, Thread_Control *heir)
{
    rtems_interrupt_level interrupts  = 0u;

    // thread switch and system not started? really?
    /* do not monitor if system not yet started */
    if ( !profilerEnabled() ||  _System_state_Current != SYSTEM_STATE_UP )
        return;

    interrupts = _sparc_disable_interrupts();
    /* disableProfiler(); */
    /* rtems_interrupt_disable(interrupts); */

    uint32_t from, to;
    if (lastIrqSource) {
        from = lastIrqSource;
        to = heir->Object.id;
        lastIrqSource = 0;
    } else {
        from = executing->Object.id;
        to = heir->Object.id;
    }
    profilePushEntry((((((from)&0x07000000)>>11)&0xe000) | ((from)&0x1fff)) & 0xffff,
                     (((to)&0x07000000)<<5) | (((to)&0x1FFF)<<16) | HOOK_TASK_SWITCH, FP_TIME_READ());

    /* enableProfiler(); */
    _sparc_enable_interrupts(interrupts);
    /* rtems_interrupt_enable(interrupts); */
}

//PROF_API
//const char*
//getRtemsObjectName(rtems_id id) {
//    static char buffer[100]; // for decoding id
//    char* ret = rtems_object_get_name(id, sizeof(buffer), buffer);
//    return (ret ? ret : "<unknown>");
//}

PROF_API
void printRtemsObject(const char* msg, Thread_Control* thr) {
    static char buffer[100]; // for decoding id
    puts(msg);
    if (thr) {
        char *ret = rtems_object_get_name(thr->Object.id, sizeof(buffer), buffer);
        if (ret) puts(ret);
        else puts("unknown");
    }
    else {
        puts("<NULL>");
    }

}

PUBLIC_PROF_API
void
__attribute__((optimize(3)))
thread_begin_extension(Thread_Control *executing) {
    printRtemsObject("Thread begin: ", executing);
}

PUBLIC_PROF_API
void
__attribute__((optimize(3)))
thread_exited_extension(Thread_Control *executing) {
        printRtemsObject("thread exit: ", executing);
//        printf("Thread \"%s\" exited. full stack unwind\n", getRtemsObjectName(executing->Object.id)); /* remove pointless print after doing something useful */
}

PUBLIC_PROF_API
bool
thread_create_ext(Thread_Control *executing, Thread_Control *created) {
    if (executing && created) {
        printRtemsObject("created:", created);
        printRtemsObject("from:", executing);
//        printf("Thr created: \"%s\" by %s\n", getRtemsObjectName(created->Object.id), getRtemsObjectName(executing->Object.id));
//        printf("NAME: %s\n", executing->Object.name.name_p);
    }
    return true;
}

PUBLIC_PROF_API
void
__attribute__((optimize(3)))
thread_terminated_extension(Thread_Control *terminated) {
    printRtemsObject("terminated:", terminated);
//    if (terminated)
//        printf("thd \"%s\" terminated. full stack unwind\n", getRtemsObjectName(terminated->Object.id));
}
// TODO: User_extensions_thread_restart_extension - needed?

PROF_INIT_FUNCTION
void
__leonProfilerInit() {
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;
    disableProfiler();
//    printf("__leonProfilerInit()\n");
    __functionProfilerInit();
    // enable rtems task_switch_extension
    rtems_id id;
    rtems_name threadExtensionName;
    rtems_extensions_table ext = {0};
    ext.thread_switch = thread_switch_extension;
    // TODO: do something withy the following - push out data
    /* ext.thread_begin = thread_begin_extension; */
    /* ext.thread_exitted = thread_exited_extension; */
    /* ext.thread_create = thread_create_ext; */
    /* ext.thread_terminate = thread_terminated_extension; */
    threadExtensionName = rtems_build_name('P', 'r', '0', 'F');
    if ( RTEMS_SUCCESSFUL != rtems_extension_create(threadExtensionName, &ext, &id) ) {
        printf("unable to install thread switch extension hook :-( cancel everything? leave the building screaming?\n");
    }
    enableProfiler();
}

#endif // #ifdef __RTEMS__
#else

// shave code

/// Interesting SVU PC counters (max 4):
/// Stalls (b16-26)
/// Branch taken count (b3 - 8) ????
/// Clock cycle count - replace timer (b2 - 4)
/// Executed instruction count (b1 - 2)
/// Stall cycle count. (b0 - 1) Stall cycles only counted when SVU_OCR[2] is zero (shave running)

// for this, global time sync is needed since there is
// one register for all shaves
// L2C_MISS_CNT, L2C_HIT_CNT

/**
 * TODO: read and send also SVU_PTH - last 16 predication decisions
 *
 */

// TODO: always read counters, multiplex them.
#if defined(FP_ENABLE_COUNTER0) || defined(FP_ENABLE_COUNTER1) \
 || defined(FP_ENABLE_COUNTER2) || defined(FP_ENABLE_COUNTER3)
#define SVU_DCR        0x1800        // Debug Control Register
#define SVU_PC0        0x1830        // Performance Counter 0
#define SVU_PCC0       0x1834        // Performance Counter 0 Control Register
#define SVU_PC1        0x1838        // Performance Counter 1
#define SVU_PCC1       0x183c        // Performance Counter 1 Control Register
#define SVU_PC2        0x1844        // Performance Counter 2
#define SVU_PCC2       0x1848        // Performance Counter 2 Control Register
#define SVU_PC3        0x184c        // Performance Counter 3
#define SVU_PCC3       0x1850        // Performance Counter 3 Control Register
#endif

// TODO: write data to profile buffer may introduce further stalls. Minimize the impact!

PROF_API
void
__shvProfilerInit(void) {

#ifdef FP_SHV_INIT_HOOK
    FP_SHV_INIT_HOOK
#endif

#if defined(FP_ENABLE_COUNTER0) || defined(FP_ENABLE_COUNTER1) \
 || defined(FP_ENABLE_COUNTER2) || defined(FP_ENABLE_COUNTER3)
    // init performance counter registers
    SET_REG_WORD(SVU_DCR, 0x1); // debug mode

#if defined(FP_ENABLE_COUNTER0)
    SET_REG_WORD(SVU_PCC0, FP_ENABLE_COUNTER0); // Usually clock cycle count (0x4)
    SET_REG_WORD(SVU_PC0, 0); // reset
#endif
#if defined(FP_ENABLE_COUNTER1)
    SET_REG_WORD(SVU_PCC1, FP_ENABLE_COUNTER1);
    SET_REG_WORD(SVU_PC1, 0); // reset
#endif
#if defined(FP_ENABLE_COUNTER2)
    SET_REG_WORD(SVU_PCC2, FP_ENABLE_COUNTER2);
    SET_REG_WORD(SVU_PC2, 0); // reset
#endif
#if defined(FP_ENABLE_COUNTER3)
    SET_REG_WORD(SVU_PCC3, FP_ENABLE_COUNTER3);
    SET_REG_WORD(SVU_PC3, 0); // reset
#endif
#endif // if any performance counter defined
}

// TODO: current workaround for constructors list not being called on all SHAVE programming paradigms
// TODO: remove this when converging on SHAVE proper initialisation support
#if MC_HAVE_WEAK_VARS
PROF_API void _preRunInit();
// FIXME: moviCompile bug: no weak variables
__attribute__((weak))
void (*)() callOnce() {
    static void (*preRunInit)() = _preRunInit;
    return preRunInit;
}
PROF_API void null() {}
#endif

// FIXME: using NextGen if this is in .ctor section, for shaves the version with no APP_
// prefix will be called that have relocation symbols for _allocateStuff from
// the LEON lib instead of shave. So no init for now...
/* PROF_INIT_FUNCTION */
void preRunInit() {
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;
    __shvProfilerInit();
    __functionProfilerInit();
    /* extern ShDrvShaveDataStruct GlbShaveData; */
    /* printf("DYN: %p and %p\n", &GlbShaveData, __globalShaveData); */
#ifdef DYNAMIC_APPLICATION
// TODO
#endif
    enableProfiler();
}

/// hook called at each function entry
PUBLIC_PROF_API
void
__attribute__((nothrow))
// not yet supported by the compiler __attribute__((optimize(3)))
__cyg_profile_func_enter(void *func, void *caller __attribute__((unused))) {
    /* return; */
    preRunInit();
    profilePushEntry((uint32_t)(func), HOOK_FUNC_ENTRY, FP_TIME_READ());

    // call UDF hook (if defined)
#ifdef FP_SHV_FCT_ENTER_HOOK
    FP_SHV_FCT_ENTER_HOOK
#endif

    // by design, PC0 is configured for clock cycle, thus this is the last read
    // on function entry and the first on function exit
    // on future release, PC0 will replace the timer.
#if defined(FP_ENABLE_COUNTER3)
    profilePushEntry((uint32_t)(SVU_PC3), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC3));
#endif
#if defined(FP_ENABLE_COUNTER2)
    profilePushEntry((uint32_t)(SVU_PC2), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC2));
#endif
#if defined(FP_ENABLE_COUNTER1)
    profilePushEntry((uint32_t)(SVU_PC1), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC1));
#endif
#if defined(FP_ENABLE_COUNTER0)
    // TODO: use this instead of free running counter
    profilePushEntry((uint32_t)(SVU_PC0), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC0));
#endif

}

PUBLIC_PROF_API
void
__attribute__((nothrow))
// not yet supported by the  compiler __attribute__((optimize(3)))
__cyg_profile_func_exit(void *func, void *caller __attribute__((unused))) {
    /* return; */
#if defined(FP_ENABLE_COUNTER0)
    profilePushEntry((uint32_t)(SVU_PC0), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC0));
#endif
#if defined(FP_ENABLE_COUNTER1)
    profilePushEntry((uint32_t)(SVU_PC1), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC1));
#endif
#if defined(FP_ENABLE_COUNTER2)
    profilePushEntry((uint32_t)(SVU_PC2), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC2));
#endif
#if defined(FP_ENABLE_COUNTER3)
    profilePushEntry((uint32_t)(SVU_PC3), HOOK_MEM_VALUE, GET_REG_WORD_VAL(SVU_PC3));
#endif

    // call UDF hook
#ifdef FP_SHV_FCT_EXIT_HOOK
    FP_SHV_FCT_EXIT_HOOK
#endif

    profilePushEntry((uint32_t)(func), HOOK_FUNC_EXIT, FP_TIME_READ());
}
#endif



// TODO: sample profiling
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/**
 * 1. Update DrvSvu.h to call our hooks from DrvSvuStart()/DrvSvuSwiHalted()
 * 2. move code from leon/sampleProfiler.c here guarded by defines
 * 3. add also guards for function profile
 * 4. for trace do not use guards, code is there... - in fact in the proposal it is ready for this
 * 5. solve ProfileBufferEntry structure which is different for each profile type
 */



// TODO: trace profiling
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#endif
