#if defined(LRT_PROFILE) || defined(LOS_PROFILE)
#ifndef __RTEMS__

#include "functionProfiler.h"

#include <swcLeonUtils.h>
#include <stdio.h>


static uint32_t patchTrapHandlerForIrqProfiling();

// TODO: save in bbuffer also caller for the cases when profiling is not enabled for everything
// or is disabled for a while, etc....
inline void
__attribute__((no_instrument_function))
__attribute__((nothrow))
__attribute__((optimize(3)))
__cyg_profile_func_enter (void *func,  void * caller __attribute__((unused)))
{
    uint32_t pil;
    if (!profilerEnabled()) return;
    /* disableProfiler(); */
    pil = swcLeonSetPIL(15);

    IN(func);
#ifdef FP_LEON_FCT_ENTER_HOOK
    FP_LEON_FCT_ENTER_HOOK
#endif

    swcLeonSetPIL(pil);
    /* enableProfiler(); */
}

inline void
__attribute__((no_instrument_function))
__attribute__((nothrow))
__attribute__((optimize(3)))
__cyg_profile_func_exit (void *func, void *caller __attribute__((unused)))
{
    uint32_t pil;
    if (!profilerEnabled()) return;
    /* disableProfiler(); */
    pil = swcLeonSetPIL(15);

#ifdef FP_LEON_FCT_EXIT_HOOK
    FP_LEON_FCT_EXIT_HOOK
#endif
    OUT(func);

    swcLeonSetPIL(pil);
    /* enableProfiler(); */
}


static int irqSource = 0;
static int nesting = 0;

static void
__attribute__((no_instrument_function))
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

      pil = swcLeonSetPIL(15);
      // 0x11 .. 0x1E (0x1F - reserved)
      int newIrqSource = (irq_src & 0xF) | 0xE000;
      int oldIrqSource = irqSource;
      irqSource = newIrqSource;
      nesting++;
      CTX_SWITCH(oldIrqSource, newIrqSource << 16);
      swcLeonSetPIL(pil);


      // actually call the handler
      (*irq_handler)( irq_src );

      swcLeonSetPIL(15);
      if (--nesting == 0)
        irqSource = 0;
      CTX_SWITCH(newIrqSource, oldIrqSource << 16);
      swcLeonSetPIL(pil);
}

// After an idea from Mircea Horia Ionica
// encoding of the instruction we need to patch
// call %l6, aka jmpl %l6+%g0, %o7
#define IRQ_CALL_INSTR 0x9fc58000
// return the address of the instruction that was patched, 0 on error
static uint32_t
__attribute__((no_instrument_function))
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



uint32_t fpInitCode = 0;

void
__attribute__((no_instrument_function))
__attribute__((constructor))
__leonProfilerInit(void) {
    __functionProfilerInit();
    __computeOverhead();
    fpInitCode = patchTrapHandlerForIrqProfiling(); // TODO: check result
    enableProfiler();
}

#endif // #ifndef __RTEMS__
#endif // #if defined(LRT_PROFILE) || defined(LOS_PROFILE)
