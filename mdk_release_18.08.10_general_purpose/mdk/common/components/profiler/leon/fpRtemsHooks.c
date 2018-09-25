#if defined(LRT_PROFILE) || defined(LOS_PROFILE)

#ifdef __RTEMS__

#include "functionProfiler.h"

#include <OsDrvSvu.h>
#include <rtems/score/sysstate.h>
#include <rtems/extension.h> // rtems_extension_create
#if !defined(MA2480) && !defined(MA2485)
#include <OsDrvTimer.h>
#endif

// FIXME: potential problem:
// for nested interrupts, in irq1 is raised irq2, the ICB.SRC is updated, and
// then after irq2 handler finishes then returning into irq1, the getInterruptSource()
// will be false. add an array of int representing the level - increment on entry, decrement on exit

inline static
__attribute__((no_instrument_function))
__attribute__((optimize(3)))
uint32_t getInterruptSource()
{
    if (rtems_interrupt_is_in_progress()) {
        return (GET_REG_WORD_VAL(
#ifdef LOS_PROFILE
            ICB0_SRC_ADR
#else
            ICB1_SRC_ADR
#endif
            ) & 0xF ) | 0x07000000; // use thread API=111 to signal this is an irq
    }

    return 0;
}

// TODO: use __attribute__ ((ifunc ("profiler_entry_exit_etc"))) to add the init stuff
// TODO: __attribute__((nonnull (1, 2))) or simply __attribute__((nonnull))


static uint32_t lastIrqSource = 0;
static uint32_t taskBeforeIrq = 0;

/// @brief Enter hook function, instrumentation call for entry of the fucntion
/// @param[in] func          - address of the start of the current function
/// @param[in] caller        - address of the function's call site
/// @attribute no_instrument_function - tells to compiler - don't instrument this function calls
/// for this function
/// @attribute optimize(3) - try to max optimize the code no matter the global options
/// @return void
///
void __attribute__((no_instrument_function))
__attribute__((optimize(3)))
__cyg_profile_func_enter (void *func, void *caller __attribute__((unused)))
{
    rtems_interrupt_level interrupts  = 0;
    rtems_id tid;
    uint32_t currentIrqSource = getInterruptSource();

    /* do not monitor if profiler not enabled or system not yet started */
    if ( !profilerEnabled() || _System_state_Current != SYSTEM_STATE_UP )
        return;

    disableProfiler();
    rtems_interrupt_disable(interrupts);

    if (rtems_interrupt_is_in_progress()) {
        if (currentIrqSource != lastIrqSource) {
            // send a special thread switch event
            if (!taskBeforeIrq) {
                lastIrqSource = taskBeforeIrq = rtems_task_self();
            }
            RTEMS_SWITCH(lastIrqSource, currentIrqSource);
            lastIrqSource = currentIrqSource;
        }
    }
    else {
        // left blank
    }

    // tid is 32 bit value. We are interested in API26:24 and index 15:0
    tid = rtems_task_self();
    RTEMS_IN(func, tid);
	// call UDF hook
#ifdef FP_RTEMS_FCT_ENTER_HOOK
    FP_RTEMS_FCT_ENTER_HOOK
#endif
    rtems_interrupt_enable(interrupts);
    enableProfiler();
}

/// @brief Exit hook function, instrumentation call for exit of the fucntion
/// @param[in] func          - address of the start of the current function
/// @param[in] caller        - address of the function's call site
/// @attribute no_instrument_function - tells to compiler - don't instrumentation call
/// for this function
/// @attribute optimize(3) - try to max optimize the code no matter the global options
/// @return void
void __attribute__((no_instrument_function))
__attribute__((optimize(3)))
__cyg_profile_func_exit (void *func, void *caller __attribute__((unused)))
{
    rtems_interrupt_level interrupts = 0;
    rtems_id tid;

    /* do not monitor if profiler not enabled or system not yet started */
    if ( !profilerEnabled() || _System_state_Current != SYSTEM_STATE_UP )
        return;

    disableProfiler();
    rtems_interrupt_disable(interrupts);

	// call UDF hook
#ifdef FP_RTEMS_FCT_EXIT_HOOK
    FP_RTEMS_FCT_EXIT_HOOK
#endif
    tid = rtems_task_self();

    if (taskBeforeIrq && lastIrqSource && !rtems_interrupt_is_in_progress()) {
        RTEMS_SWITCH(lastIrqSource, taskBeforeIrq);
        lastIrqSource = taskBeforeIrq = 0;
    }
    RTEMS_OUT(func, tid);

    rtems_interrupt_enable(interrupts);
    enableProfiler();
}

void __attribute__((no_instrument_function))
thread_switch_extension(Thread_Control *executing, Thread_Control *heir)
{
	rtems_interrupt_level interrupts  = 0u;

	// thread switch and system not started? really?
    /* do not monitor if system not yet started */
    if ( !profilerEnabled() || _System_state_Current != SYSTEM_STATE_UP )
        return;

    disableProfiler();
	rtems_interrupt_disable(interrupts);

    RTEMS_SWITCH(executing->Object.id, heir->Object.id);
    lastIrqSource = 0;

	rtems_interrupt_enable(interrupts);
    enableProfiler();
}

/*inline*/ const char*
__attribute__((no_instrument_function))
getRtemsObjectName(rtems_id id) {
    char buffer[10]; // for decoding id
    char* ret = rtems_object_get_name(id, sizeof(buffer), buffer);
    return (ret ? ret : "<unknown>");
}

void
__attribute__((no_instrument_function))
__attribute__((optimize(3)))
thread_begin_extension(Thread_Control *executing __attribute__((unused))) {
	//printf("Thread \"%s\" begin\n", getRtemsObjectName(executing->Object.id)); /* remove pointless print after doing something useful */
}
void
__attribute__((no_instrument_function))
__attribute__((optimize(3)))
thread_exited_extension(Thread_Control *executing __attribute__((unused))) {
	//printf("Thread \"%s\" exited. full stack unwind\n", getRtemsObjectName(executing->Object.id)); /* remove pointless print after doing something useful */
}
bool
__attribute__((no_instrument_function))
thread_create_ext(Thread_Control *executing __attribute__((unused)), Thread_Control *created __attribute__((unused))) {
    //printf("Thr created: \"%s\"\n", getRtemsObjectName(created->Object.id));
    return true;
}

void
__attribute__((no_instrument_function))
__attribute__((optimize(3)))
thread_terminated_extension(Thread_Control *terminated __attribute__((unused))) {
    //printf("thd \"%s\" terminated. full stack unwind\n", getRtemsObjectName(terminated->Object.id));
}
// TODO: User_extensions_thread_restart_extension - needed?

void
__attribute__((no_instrument_function))
__attribute__((constructor))
__leonProfilerInit() {
    __functionProfilerInit();
    // enable rtems task_switch_extension
    rtems_id id;
    rtems_name threadExtensionName;
    rtems_extensions_table ext = {0};
    ext.thread_switch = thread_switch_extension;
    ext.thread_begin = thread_begin_extension;
    ext.thread_exitted = thread_exited_extension;
    ext.thread_create = thread_create_ext;
    ext.thread_terminate = thread_terminated_extension;
    threadExtensionName = rtems_build_name('P', 'r', '0', 'F');
    if ( RTEMS_SUCCESSFUL != rtems_extension_create(threadExtensionName, &ext, &id) ) {
        printf("unable to install thread switch extension hook :-( cancel everything? leave the building screaming?\n");
    }
    __computeOverhead();
    enableProfiler();
}

#endif // #ifdef __RTEMS__
#endif // #if defined(LRT_PROFILE) || defined(LOS_PROFILE)
