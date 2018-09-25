# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : Breakpoints.tcl
# Description: Breakpoints TCF interface
# Created on :
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval tcf::Breakpoints {
    namespace eval BREAKPOINT::FIELDS {
        # <string> Breakpoint ID. This is the only required property.
        set ID ID
        # <boolean> If true, the breakpoint is enabled.
        set ENABLED Enabled
        set ENABLED.TYPE json::Boolean
        # <string> The breakpoint type
        set TYPE BreakpointType
        # <string> This property contains an array of TCF context identifiers for which this breakpoint should be installed.
        set CONTEXT_IDS ContextIds
        set CONTEXT_IDS.TYPE json::StringList
        # <string> This property contains an array of contexts names (such as a process/thread name) for which this breakpoint should be installed.
        set CONTEXT_NAMES ContextNames
        set CONTEXT_NAMES.TYPE json::StringList
        # <string> This property contains all the target executable paths for which this breakpoint should be installed.
        set EXECUTABLE_PATHS ExecutablePaths
        set EXECUTABLE_PATHS.TYPE json::StringList
        # <string> If preset, defines location of the breakpoint. The expression evaluates either to a memory address or a register location.
        set LOCATION Location
        # <int> The access mode that will trigger the breakpoint. Access mode can be a bitwise OR of the values below:
        set ACCESS_MODE AccessMode
        set ACCESS_MODE.TYPE json::Integer
        # <int> The number of bytes starting at address given by the location expression. AccessMode applies to accesses within the range [location .. location+size-1]. The MaskValue and Mask applies to the bytes within the range, i.e. the breakpoint triggers when (valueof[location .. location+size-1] & Mask) == (MaskValue & Mask).
        set SIZE Size
        set SIZE.TYPE json::Integer
        # <string> Source code file name of breakpoint location.
        set FILE File
        # <int> Source code line number of breakpoint location.
        set LINE Line
        set LINE.TYPE json::Integer
        # <int> Source code column number of breakpoint location.
        set COLUMN Column
        set COLUMN.TYPE json::Integer
        # <int> A breakpoint can be qualified with a mask value which may be further refined with a mask.
        set MASK_VALUE MaskValue
        set MASK_VALUE.TYPE json::Integer
        set MASK_VALUE.FORMAT "0x%.8X"

        # <int> A mask which is bitwise ANDed with the value accessed.
        set MASK Mask
        set MASK.TYPE json::Integer
        set MASK.FORMAT "0x%.8X"
        # <number> The time value in the execution of the program at which to set the breakpoint
        set TIME Time
        set TIME.TYPE json::Number
        # <string> The scale for the time value
        set TIME_SCALE TimeScale
        # <string> The units for the time value
        set TIME_UNITS TimeUnits
        # <string> Expression that must evaluate to true before the breakpoint is triggered.
        set CONDITION Condition
        # <int> The number of times this breakpoint is to be ignored before it is triggered. The ignore count is tested after all other Location and Condition properties are validated.
        set IGNORE_COUNT IgnoreCount
        set IGNORE_COUNT.TYPE json::Integer
        # <string> An array of TCF Context identifiers representing contexts to be stopped when this breakpoint is triggered. This is an "Action" property that is used to stop contexts in addition to the one that triggered the breakpoint.
        set STOP_GROUP StopGroup
        set STOP_GROUP.TYPE json::StringList
        # <boolean> If set, results in the breakpoint being removed after it is triggered once. The default value for this property is false.
        set TEMPORARY Temporary
        set TEMPORARY.TYPE json::Boolean

        set SKIP_PROLOGUE SkipPrologue
        set SKIP_PROLOGUE.TYPE json::Boolean

        namespace eval TYPE::VALUES {
            # Software breakpoint.
            set SOFTWARE "Software"
            # Hardware breakpoint.
            set HARDWARE "Hardware"
            # Installed breakpoint type (software/hardware) deferred to agent's discretion.
            # This is the default breakpoint type. The actual type of each breakpoint instance
            # consequently installed is received as a status event.
            set AUTO "Auto"
        }

        namespace eval ACCESS_MODE::FLAGS {
            # Triggered by a read from the breakpoint location.
            set READ 0x01
            # Triggered by a write to the breakpoint location.
            set WRITE 0x02
            # Triggered by an instruction execution at the breakpoint location. Whether the breakpoint is triggered before or after the instruction execution depends on the target support for this mode. This is the default for Line and Address breakpoints.
            set EXECUTE 0x04
            # Triggered by a data change (not an explicit write) at the breakpoint location.
            set CHANGE 0x08
        }

        namespace eval TIME_VALUE::VALUES {
            # Time value in the relative time scale. This is the default value for this property. In the relative time scale, the Time property may have a negative value.
            set RELATIVE Relative
            # Time value in the absolute time scale.
            set ABSOLUTE Absolute
        }

        namespace eval TIME_UNIT::VALUES {
            # Time value in cycles. This is the default type.
            set CYCLE_COUNT CycleCount
            # Time value in nano seconds.
            set NANOSECONDS NanoSeconds
            # Time value in instructions.
            set INSTRUCTION_COUNT InstructionCount
        }

    }

    namespace eval STATUS::FIELDS {
        #  <array of instance status data>
        set INSTANCES Instances
        #  <string>
        set ERROR Error
        #  <string>
        set FILE File
        #  <int>
        set LINE Line
        #  <int>
        set COLUMN Column
    }

    namespace eval STATUS::INSTANCE::FIELDS {
        #  <string>
        set ERROR Error
        #  <string>
        set TYPE BreakpointType
        #  <string>
        set CONTEXT LocationContext
        #  <string>
        set ADDRESS Address
        set ADDRESS.FORMAT 0x%.8X
        # <integer>
        set HIT_COUNT HitCount

        # <string>
        set CONDITION_ERROR ConditionError

        namespace eval TYPE::VALUES {
            set SOFTWARE "Software"
            set HARDWARE "Hardware"
            set AUTO "Auto"
        }
    }

    namespace eval BREAKPOINT {

        proc fromFields {fieldsDict} {
            json::ObjectFromFieldDict $fieldsDict
        }

    }

    namespace eval set {

        proc jsonList {breakpoints} {
            ::tcf::protocol::jsonCommand \
                Breakpoints set \
                [json::List $breakpoints]
        }

        proc json {args} {
            jsonList $args
        }

        proc fromObjects {args} {
            set jsonObjList [list]
            foreach arg $args {
                lappend jsonObjList \
                    [[namespace parent]::BREAKPOINT::fromFields $arg]
            }
            jsonList $jsonObjList
        }

    }

    namespace eval add {

        proc json {bp} {
            ::tcf::protocol::jsonCommand \
                Breakpoints add $bp
        }

        proc fromObject {dict} {
            json [[namespace parent]::BREAKPOINT::fromFields $dict]
        }

        proc fromFields {args} {
            fromObject $args
        }

    }

    namespace eval change {

        proc json {bp} {
            ::tcf::protocol::jsonCommand \
                Breakpoints change $bp
        }

        proc fromObject {object} {
            json [[namespace parent]::BREAKPOINT::fromFields $object]
        }

        proc fromFields {args} {
            fromObject $args
        }

    }

    proc enable {bpList} {
        ::tcf::protocol::jsonCommand \
            Breakpoints enable \
            [json::StringList $bpList]
    }

    proc disable {bpList} {
        ::tcf::protocol::jsonCommand \
            Breakpoints disable \
            [json::StringList $bpList]
    }

    proc remove {bpList} {
        ::tcf::protocol::jsonCommand \
            Breakpoints remove \
            [json::StringList $bpList]
    }

    proc getIDs {} {
        ::tcf::protocol::jsonCommand \
            Breakpoints getIDs
    }

    proc getProperties {id} {
        ::tcf::protocol::jsonCommand \
            Breakpoints getProperties [json::String $id]
    }

    proc getStatus {id} {
        ::tcf::protocol::jsonCommand \
            Breakpoints getStatus [json::String $id]
    }

    # TODO: capabilities fields
    proc getCapabilities {contextId} {
        ::tcf::protocol::jsonCommand \
            Breakpoints getCapabilities [json::String $contextId]
    }

    namespace eval Events {
        # E • Breakpoints • status • <string: breakpoint ID> • <breakpoint status> •
        ::tcf::protocol::JsonEvent2 Breakpoints status
        # E • Breakpoints • contextAdded • <array of breakpoints> •
        ::tcf::protocol::JsonEvent Breakpoints contextAdded
        # E • Breakpoints • contextChanged • <array of breakpoints> •
        ::tcf::protocol::JsonEvent Breakpoints contextChanged
        # E • Breakpoints • contextRemoved • <array of breakpoint IDs> •
        ::tcf::protocol::JsonEvent Breakpoints contextRemoved
    }

} ; # namespace eval tcf::Breakpoints
