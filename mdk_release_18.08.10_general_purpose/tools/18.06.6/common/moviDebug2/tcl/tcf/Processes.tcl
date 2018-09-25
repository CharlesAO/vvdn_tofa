namespace eval tcf::Processes {
    namespace eval CONTEXT::FIELDS {
        # <string> - ID of the context, same as getContext command argument.
        set ID ID
        # <string> - parent context ID.
        set PARENT_ID ParentID
        # <string> - process name. Client UI can show this name to a user.
        set NAME Name
        # <boolean> - true if the context is attached to debugger.
        set ATTACHED Attached
        # <boolean> - true if the service can terminate the process.
        set CanTerminate CanTerminate
        # <string> - process standard input stream ID.
        set STD_IN_ID StdInID
        # <string> - process standard output stream ID.
        set STD_OUT_ID StdOutID
        # <string> - process standard error stream ID.
        set STD_ERR_ID StdErrID
    }

    proc getContext {contextId} {
        tcf::protocol::jsonCommand \
            Processes getContext \
            [json::String $contextId]
    }

    namespace eval START_PARAMS::FIELDS {
        set ATTACH Attach
        set ATTACH.TYPE json::Boolean
        set STOP_AT_ENTRY StopAtEntry
        set STOP_AT_ENTRY.TYPE json::Boolean
        set STOP_AT_MAIN StopAtMain
        set STOP_AT_MAIN.TYPE json::Boolean
        set USE_TERMINAL UseTerminal
        set USE_TERMINAL.TYPE json::Boolean
    }

    proc StartParams {args} {

    }

    proc start {
                programImage
                {workingDir {}}
                {commandLine {}}
                {environment {}}
                {attach true}
    } {
        tcf::protocol::jsonCommand \
            Processes start \
            [json::String $workingDir] \
            [json::String $programImage] \
            [json::StringList $commandLine] \
            [json::StringList $environment] \
            [json::Boolean $attach]
    }

    proc attach {contextId} {
        tcf::protocol::jsonCommand \
            Processes attach \
            [json::String $contextId]
    }

    proc detach {contextId} {
        tcf::protocol::jsonCommand \
            Processes detach \
            [json::String $contextId]
    }

    proc terminate {contextId} {
        tcf::protocol::jsonCommand \
            Processes terminate \
            [json::String $contextId]
    }

    proc getChildren {contextId {attachedOnly true}} {
        tcf::protocol::jsonCommand \
            Processes getChildren \
            [json::String $contextId] \
            [json::Boolean $attachedOnly]
    }

    namespace eval Events {
        ::tcf::protocol::JsonEvent Processes exited
    }
}

namespace eval tcf::ProcessesV1 {

    proc start {
                programImage
                {workingDir {}}
                {commandLine {}}
                {environment {}}
                {startParams {Attach true}}
    } {
        tcf::protocol::jsonCommand \
            ProcessesV1 start \
            [json::String $workingDir] \
            [json::String $programImage] \
            [json::StringList $commandLine] \
            [json::StringList $environment] \
            [json::Object $startParams]
    }

}
