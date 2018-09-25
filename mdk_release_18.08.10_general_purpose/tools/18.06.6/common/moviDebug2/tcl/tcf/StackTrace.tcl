# This script implements the tcf json request documented here:
# http://git.eclipse.org/c/tcf/org.eclipse.tcf.git/plain/docs/TCF%20Service%20-%20Stack%20Trace.html

namespace eval tcf::StackTrace {

    namespace eval CONTEXT::FIELDS {
        # <string> - ID of the context, same as getContext command argument.
        set ID ID
        # <string> - ID of a parent context.
        set PARENT_ID ParentID
        # <string> - process ID.
        set PROCESS_ID ProcessID
        # <string> - context name if context is a stack.
        set NAME Name
        # <boolean> - true if the frame is top frame on a stack.
        set TOP_FRAME TopFrame
        # <number> - frame index. Topmost (current) frame has index 0.
        set INDEX Index
        # <boolean> - true if the frame data was computed using symbols info, false or not set if the data was collected using stack crawl logic.
        set WALK Walk
        # <number> - frame pointer - memory address of stack frame.
        set FRAME_PTR FP
        set FRAME_PTR.FORMAT 0x%.8X
        # <number> - return pointer - return address.
        set RETURN_PTR RP
        set RETURN_PTR.FORMAT 0x%.8X
        # <number> - instruction pointer - memory address of current instruction.
        set INSTRUCTION_PTR IP
        set INSTRUCTION_PTR.FORMAT 0x%.8X
        # <number> - function arguments count
        set ARGS_COUNT ArgsCnt
        # <number> - memory address of function arguments
        set ARGS_ADDR ArgsAddr
        set ARGS_ADDR.FORMAT 0x%.8X
        # <number> - source code location of the frame
        set CODE_AREA CodeArea
        # <number> - function symbol ID.
        set FUNC_ID FuncID

        # undocumented
        set LEVEL Level
    }

    # TCF doc:
    # Retrieves context info for given context IDs. Command allows to query
    # multiple contexts at once.  Stack Trace context represents single stack
    # frame. If target supports more than one stack per thread, each stack is
    # also represented  by a separate context.
    # C • <token> • StackTrace • getContext • <array of context IDs> •
    # R • <token> • <array of context data> • <error report> •
    proc getContext {args} {
        lindex [tcf::protocol::jsonListCommand \
            1 1 \
            StackTrace getContext \
            [json::StringList $args] \
        ] 0
    }

    # TCF doc:
    # Retrieves stack trace context ID list. Parent context usually corresponds to an execution thread. Some targets have more then one stack.
    # In such case children of a thread are stacks, and stack frames are deeper in the hierarchy - they can be retrieved with additional getChildren commands.
    #
    # The command will fail if parent thread is not suspended. Client can use Run Control service to suspend a thread.
    proc getChildren { parentCtxId } {
        tcf::protocol::jsonCommand \
            StackTrace getChildren \
            [json::String $parentCtxId ]
    }

    # TCF doc:
    # Retrieves a range of stack trace context IDs. Parent context usually corresponds to an execution thread.
    #
    # Note: to allow partial and incremental stack tracing, IDs ordering in the range is reversed relative to getChildren command order.
    # For example, range 0..1 represents last two stack frames: current frame (top of the stack) and previous one.
    #
    # The command will fail if parent thread is not suspended. Client can use Run Control service to suspend a thread.
    proc getChildrenRange { parentCtxId rangeStart rangeEnd } {
        tcf::protocol::jsonCommand \
            StackTrace getChildrenRange \
            [json::String $parentCtxId] \
            [json::Integer $rangeStart] \
            [json::Integer $rangeEnd]
    }

} ;# namespace eval tcf::StackTrace

namespace eval tcf {

    proc isFrameContextId {contextId} {
        regexp {^FP[0-9]+\.(.*)$} $contextId
    }

    proc getFrameContextParentId {frameContextId} {
        set frameContext [lindex [tcf::StackTrace::getContext $frameContextId] 0]
        set default $frameContextId
        fields::getNamedField StackTrace::CONTEXT $frameContext PARENT_ID $default
    }

    proc getFrameField {symContextDict field {default {}} } {
        fields::getNamedField StackTrace::CONTEXT $symContextDict $field $default
    }

    proc getFrameFieldList {symContextDict fieldList {defaults {}} } {
        fields::getNamedFieldList StackTrace::CONTEXT $symContextDict $fieldList $defaults
    }

    proc getFrameFieldDict {symContextDict} {
        fields::getNamedFieldDict StackTrace::CONTEXT $symContextDict
    }

    proc getTopStackFrameId {contextId} {
        getStackFrameId $contextId 0
    }

    proc getStackFrameId {contextId frameNum} {
        StackTrace::getChildrenRange $contextId $frameNum $frameNum
    }

} ; # namespace eval tcf

