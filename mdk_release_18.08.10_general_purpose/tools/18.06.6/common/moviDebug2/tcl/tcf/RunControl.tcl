namespace eval tcf::RunControl {
    namespace eval CONTEXT::FIELDS {
        # <string> - ID of the context, same as getContext command argument.
        set ID ID
        # <string> - ID of a parent context.
        set PARENT_ID ParentID
        # <string> - ID of a process (memory space) of the context.
        set PROCESS_ID ProcessID
        # <string> - ID of a context that created (started) this context.
        set CREATOR_ID CreatorID
        # <string> - human redable context name.
        set NAME Name
        # <boolean> - true if the context is a container. Executing resume or
        # suspend command on a container causes all its children to resume or suspend.
        set IS_CONTAINER IsContainer
        # <boolean> - true if the context is an execution context, therefore has an
        # execution state, like state of a program counter (PC). Only context
        # that has a state can be resumed or suspended.
        set HAS_STATE HasState
        # <boolean> - true if Suspend command is supported for this context.
        # It does not mean that the command can be executed successfully in the
        # current state of the context. For example, the command still can fail
        # if context is already suspended.
        set CAN_SUSPEND CanSuspend
        # <int: bitset of resume modes> - for each resume mode, corresponding bit
        # is '1' if Resume command mode is supported for this context, and '0'
        # otherwise. It does not mean that the command can be executed
        # successfully in the current state of the context.
        # For example, the command still can fail if context is already resumed.
        set CAN_RESUME CanResume
        # <int: bitset of resume modes> - for each resume mode, corresponding bit is '1'
        set CAN_RESUME.FORMAT %#.8x
        # if Resume command mode with count other then 1 is supported by the context.
        set CAN_COUNT CanCount
        # <boolean> - true if Terminate command is supported by the context.
        set CAN_TERMINATE CanTerminate
        # <boolean> - true if Detach command is supported by the context.
        set CAN_DETACH CanDetach
        # <string> - context ID of a run control group that contains the context.
        # Members of same group are always suspended and resumed together:
        # resuming/suspending a context resumes/suspends all members of the group
        set RC_GROUP RCGroup
        # <string> - context ID of a breakpoints group that contains the context.
        # Members of same group share same breakpoint instances: a breakpoint is
        # planted once for the group, no need to plant the breakpoint for each member of the group
        set BP_GROUP BPGroup
        # <string> - Context ID of a symbols group that contains the context.
        # Members of a symbols group share same symbol reader configuration settings,
        # like user defined memory map entries and source lookup info
        set SYMBOLS_GROUP SymbolsGroup

        # undocumented
        set WORD_SIZE WordSize
    }

    proc getContext {{contextId {}}} {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand RunControl getContext [json::String $contextId]
        }
    }

    proc getChildren {{contextId {}}} {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand RunControl getChildren [json::String $contextId]
        }
    }

    proc suspend {contextId} {
        tcf::protocol::jsonListCommand \
            0 0 \
            RunControl suspend \
            [json::String $contextId]
    }

    proc resume {contextId mode {count 1}} {
        tcf::protocol::jsonListCommand \
            0 0 \
            RunControl resume \
            [json::String $contextId] \
            [json::Number [getResumeMode $mode]] \
            [json::Number $count] \
            null
    }

    proc getResumeMode {name} {
        if [string is integer $name] {
            return $name
        }
        set [namespace current]::RESUME::$name
    }

    proc resumeRange {contextId mode rangeStart rangeEnd {count 1}} {
        tcf::protocol::jsonCommand \
            RunControl resume \
            [json::String $contextId] \
            [json::Number [getResumeMode $mode]] \
            [json::Number $count] \
            [json::Object [dict create\
                RangeStart [json::Number $rangeStart]\
                RangeEnd   [json::Number $rangeEnd]
            ]]
    }

    namespace eval RESUME {
        # resume normal execution.
        # Execution will continue until suspended by command or breakpoint.
        set NORMAL 0
        # step over a single instruction. If instruction is function call,
        # execution continues until control returns from the function.
        set STEP_OVER_INSTR 1
        # single instruction in given context.
        set STEP_INSTR 2
        # resume execution of given context until control reaches
        # instruction that belongs to a different line of source code,
        # but runs any functions called at full speed. Error is returned
        # if line number information not available.
        set STEP_OVER_LINE 3
        # resumes execution of given context until control reaches
        # instruction that belongs to a different line of source code.
        # If a function is called, stop at first line of the function code.
        # Error is returned if line number information not available.
        set STEP_INTO_LINE 4
        # resume execution of given context until control returns from
        # current function.
        set STEP_RETURN 5
        set STEP_OUT 5

        # step over instructions until PC is outside the specified range.
        # A function call within the range is considered to be in range.
        set STEP_OVER_RANGE 12
        # step instruction until PC is outside the specified range for any reason.
        set UNTIL_OUT_OF_RANGE 13
        # run until the context becomes active - scheduled to run on a target CPU.
        set UNTIL_ACTIVE 16

        namespace eval REVERSE {
            # resume backward execution. Execution will continue until suspended by
            # command or breakpoint.
            set NORMAL 6
            # reverse step over a single instruction.
            set STEP_OVER_INSTR 7
            # reverse step into a single instruction in the given context. This
            # effectively "un-executes" the previous instruction.
            set STEP_INTO_INSTR 8
            # resume backward execution of the context until control reaches an
            # instruction that belongs to a different source line. If the line
            # contains a function call then don't stop until get out of the function.
            # Error is returned if line number information not available.
            set STEP_OVER_LINE 9
            # resume backward execution of the context until control
            # reaches an instruction that belongs to a different line of source code.
            # If a function is called, stop at the beginning of the last line of the
            # function code. Error is returned if line number information not available.
            set STEP_INTO_LINE 10
            # resume backward execution of the context until control reaches the
            # point where the current function was called.
            set STEP_CALL 11
            set STEP_OUT 11
            # resume backward execution until PC is outside the specified range.
            # A function call within the range is considered to be in range.
            set STEP_OVER_RANGE 14
            # resume backward execution until PC is outside the specified range
            # for any reason.
            set UNTIL_OUT_OF_RANGE 15
            #    17 - run reverse until the context becomes active.
            set UNTIL_ACTIVE 17
        }
    }

    # R • <token> • <error report> • <boolean: suspended> • <int: PC> • <string: last state change reason> • <state data> •
    proc getState {contextId} {
        ::tcf::protocol::blockEvents {
            ::tcf::protocol::jsonListCommand \
                4 0 \
                RunControl getState \
                [json::String $contextId]
        }
    }

    namespace eval STATE {
        namespace eval INDEX {
            set SUSPENDED 0
            set PC 1
            set PC.FORMAT {0x%.8X}
            set REASON 2
            set DATA 3
            set STATE 3 ; # for backwards compatibility

            namespace eval REASON::VALUES {
                #  context suspended by command.
                set SUSPENDED Suspended
                #  context suspended by step command.
                set STEP Step
                #  context suspended by breakpoint.
                set BREAKPOINT Breakpoint
                #  context suspended by exception.
                set EXCEPTION Exception
                #  context suspended as part of container.
                set CONTAINER Container
                #  context suspended by watchpoint (data breakpoint).
                set WATCHPOINT Watchpoint
                #  context suspended because it received a signal.
                set SIGNAL Signal
                #  context suspended because a shared library is loaded or unloaded.
                set SHARED_LIBRARY Shared\ Library
                #  context suspended because of an error in execution environment.
                set ERROR Error
            }
        } ; # namespace eval INDEX

        namespace eval DATA::FIELDS {
            #  <int> - signal that caused the context to become suspended
            set SIGNAL Signal
            #  <string> - name of the signal that caused the context to become suspended
            set SIGNAL_NAME SignalName
            #  <string> - description of the signal that caused the context to become suspended
            set SIGNAL_DESCRIPTION SignalDescription
            #  <array of string> - IDs of breakpoints that were triggered by the context
            set BPS BPs
            #  <object> - error report that describes a reason why program counter of the context is not available
            set PC_ERROR PCError
            #  <boolean> - true if the context is running in reverse
            set REVERSING Reversing

            # <string> Not empty if inactive/not running
            set STATE_NAME StateName

        } ; # namespace eval DATA::FIELDS

    } ; # namespace eval STATE

    namespace eval Events {
        # E • RunControl • contextAdded • <array of context data> •
        ::tcf::protocol::JsonEvent RunControl contextAdded
        # E • RunControl • contextChanged • <array of context data> •
        ::tcf::protocol::JsonEvent RunControl contextChanged
        # E • RunControl • contextRemoved • <array of context IDs> •
        ::tcf::protocol::JsonEvent RunControl contextRemoved
        # E • RunControl • contextSuspended • <string: context ID> • <int: PC> • <string: reason> • <state data> •
        ::tcf::protocol::JsonEvent2 RunControl contextSuspended
        # E • RunControl • contextResumed • <string: context ID> •
        ::tcf::protocol::JsonEvent RunControl contextResumed
        # E • RunControl • contextException • <string: context ID> • <string: description> •
        ::tcf::protocol::JsonEvent2 RunControl contextException
        # E • RunControl • containerSuspended • <string: context ID> • <int: PC> • <string: reason> • <state data> • <array of context IDs> •
        ::tcf::protocol::JsonEvent2 RunControl containerSuspended
        # E • RunControl • containerResumed • <array of context IDs> •
        ::tcf::protocol::JsonEvent RunControl containerResumed

        # Undocumented
        ::tcf::protocol::JsonEvent RunControl contextStateChanged
    }
}

