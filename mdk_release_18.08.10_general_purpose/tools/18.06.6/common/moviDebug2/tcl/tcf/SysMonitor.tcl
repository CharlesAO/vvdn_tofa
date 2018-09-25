namespace eval tcf::SysMonitor {

    namespace eval CONTEXT::FIELDS {
        # <string> - ID of the context, same as getContext command argument.
        set ID ID
        # <string> - parent context ID.
        set PARENT_ID ParentID
        # <string> - current working directory of the process.
        set CWD CWD
        # <string> - the process's root directory (as set by chroot).
        set ROOT_DIR Root
        # <int> - User ID of the process owner.
        set UID UID
        # <int> - Group ID of the process owner.
        set UGID UGID
        # <string> - user name of the process owner.
        set USER_NAME UserName
        # <string> - group name of the process owner.
        set GROUP_NAME GroupName
        # <int> - system process ID.
        set PID PID
        # <string> - executable file of the process.
        set FILE File
        # <string> - one character from the string "RSDZTW" where R is running, S is sleeping in an interruptible wait, D is waiting in uninterruptible disk sleep, Z is zombie, T is traced or stopped (on a signal), and W is paging.
        set STATE State
        # <int> - system ID of the parent process.
        set PPID PPID
        # <int> - the process group ID of the process.
        set PGRP PGRP
        # <int> - the session ID of the process.
        set Session Session
        # <int> - the tty the process uses.
        set TTY TTY
        # <int> - the process group ID of the process which currently owns the tty that the process is connected to.
        set TGID TGID
        # <int> - ID of a process that has attached this process for tracing or debugging.
        set TRACER_PID TracerPID
        # <int> - the kernel flags word of the process. Details depend on the kernel.
        set FLAGS Flags
        # <int> - the number of minor faults the process has made which have not required loading a memory page from disk.
        set MINOR_FAULTS MinFlt
        # <int> - the number of minor faults that the process's waited-for children have made.
        set CHILD_MINOR_FAULTS CMinFlt
        # <int> - the number of major faults the process has made which have required loading a memory page from disk.
        set MAJOR_FAULTS MajFlt
        # <int> - the number of major faults that the process's waited-for children have made.
        set CHILD_MAJOR_FAULTS CMajFlt
        # <int> - the number of milliseconds that this process has been scheduled in user mode.
        set USER_TIME UTime
        # <int> - the number of milliseconds that this process has been scheduled in kernel mode.
        set SYSTEM_TIME STime
        # <int> - the number of jiffies that this process's waited-for children have been scheduled in user mode.
        set CHILD_USER_TIME CUTime
        # <int> - the number of jiffies that this process's waited-for children have been scheduled in user mode.
        set CHILD_SYSTEM_TIME CSTime
        # <int> - the standard nice value.
        set PRIORITY Priority
        # <int> - the nice value.
        set NICE Nice
        # <int> - the time in milliseconds before the next SIGALRM is sent to the process due to an interval timer.
        set INTERVAL_TIMER_REAL_VALUE ITRealValue
        # <int> - the time in milliseconds the process started after system boot.
        set START_TIME StartTime
        # <int> - virtual memory size in bytes.
        set VIRT_SIZE VSize
        # <int> - memory pages size in bytes.
        set PAGE_SIZE PSize
        # <int> - resident Set Size# number of pages the process has in real memory, minus used for administrative purposes. This is just the pages which count towards text, data, or stack space. This does not include pages which have not been demand-loaded in, or which are swapped out.
        set RESIDENT_SET_SIZE RSS
        # <int> - current limit in bytes on the rss of the process.
        set RESIDENT_SET_SIZE_LIMIT RLimit
        # <int> - the address above which program text can run.
        set CODE_START CodeStart
        # <int> - the address below which program text can run.
        set CODE_END CodeEnd
        # <int> - the address of the start of the stack.
        set STACK_STACK StackStart
        # <int> - the bitmap of pending signals.
        set SIGNALS Signals
        # <int> - the bitmap of blocked signals.
        set SIGNALS_BLOCKED SigBlock
        # <int> - the bitmap of ignored signals.
        set SIGNALS_IGNORED SigIgnore
        # <int> - the bitmap of caught signals.
        set SIGNALS_CAUGHT SigCatch
        # <int> - this is the "channel" in which the process is waiting. It is the address of a system call, and can be looked up in a namelist if you need a textual name.
        set WCHAN WChan
        # <int> - number of pages swapped.
        set NSWAP NSwap
        # <int> - cumulative NSwap for child processes.
        set CHILD_NSWAP CNSwap
        # <int> - signal to be sent to parent when this process exits.
        set EXIT_SIGNAL ExitSignal
        # <int> - CPU number last executed on.
        set PROCESSOR Processor
        # <int> - real-time scheduling priority.
        set REALTIME_PRIORITY RTPriority
        # <int> - scheduling policy.
        set POLICY Policy
    }

    proc getContext {contextId} {
        ::tcf::protocol::jsonCommand \
            SysMonitor getContext \
            [json::String $contextId]
    }

    proc getChildren {{contextId {}}} {
        ::tcf::protocol::jsonCommand \
            SysMonitor getChildren \
            [json::String $contextId]
    }

    proc getCommandLine {contextId} {
        ::tcf::protocol::jsonCommand \
            SysMonitor getCommandLine \
            [json::String $contextId]
    }

    proc getEnvironment {contextId} {
        ::tcf::protocol::jsonCommand \
            SysMonitor getEnvironment \
            [json::String $contextId]
    }

}