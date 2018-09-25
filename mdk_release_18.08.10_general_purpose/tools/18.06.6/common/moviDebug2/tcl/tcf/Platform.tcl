# This script implements the Platform service, which is moviDebug2 specific (not
# tcf specific)

namespace eval tcf::Platform {

    namespace eval CONTEXT::FIELDS {
        # <string> - ID of the context, same as getContext command argument.
        set ID Id
        # <string> - ID of a parent context.
        set PARENT_ID ParentID
        # <string> - platform ID.
        set PROCESS_ID PlatformID
        # <string> - context name if context is a platform.
        set NAME Name
    }

    proc boardReset { contextId } {
        tcf::protocol::jsonCommand \
            Platform boardReset [json::String $contextId]
    }

    proc ddrInit { contextId } {
        tcf::protocol::jsonCommand \
            Platform ddrInit [json::String $contextId]
    }

    proc getDefaultMainCore { contextId } {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand \
                Platform getDefaultMainCore [json::String $contextId]
        }
    }

    proc getMainCore { contextId } {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand \
                Platform getMainCore [json::String $contextId]
        }
    }

    proc setMainCore { platformContextId coreContextId } {
        tcf::protocol::blockEvents {
            tcf::protocol::jsonCommand \
                Platform setMainCore \
                    [json::String $platformContextId] \
                    [json::String $coreContextId]
        }
    }

    proc verify { contextId elfFileName {options {}}} {
        tcf::protocol::jsonCommand \
            Platform verify [json::String $contextId] [json::String $elfFileName] [json::Object $options]
    }

    proc addPeripheral {contextId json_object} {
        tcf::protocol::jsonCommand \
            Platform addPeripheral [json::String $contextId] $json_object
    }

    namespace eval Events {
        # E • Platform • boardReset • <string: context ID> • <extra data> •
        ::tcf::protocol::JsonEvent2 Platform boardReset
        # E • Platform • ddrInit • <string: context ID> • <extra data> •
        ::tcf::protocol::JsonEvent2 Platform ddrInit
        # E • Platform • changeWorkDir • <string: context ID> • <string: workingDir> •
        ::tcf::protocol::JsonEvent2 Platform changeWorkDir
    } ; # namespace eval Events

    namespace eval SyncEvents {
        # E • Platform • beforeBoardReset • <string: context ID> • <extra data> •
        ::tcf::protocol::JsonSyncEvent2 Platform beforeBoardReset
        # E • Platform • boardReset • <string: context ID> • <extra data> •
        ::tcf::protocol::JsonSyncEvent2 Platform boardReset
        # E • Platform • ddrInit • <string: context ID> • <extra data> •
        ::tcf::protocol::JsonSyncEvent2 Platform ddrInit
        # E • Platform • changeWorkDir • <string: context ID> • <string: workingDir> •
        ::tcf::protocol::JsonSyncEvent2 Platform changeWorkDir
    } ; # namespace eval SyncEvents

}