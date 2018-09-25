# ********************************************************s)*******************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/breakpoint.tcl
#              TODO: rename to cli/breakpoints.tcl
# Description: moviDebug2 CLI `breakpoint` command
# Created on : Aug 27, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::Breakpoints {
        namespace path ::mdbg

        cli::MultiCmdArgParser create ArgumentParser -auto -brief "Manage breakpoints." -description {
            The command allows management of all types of TCF breakpoints/watchpoints and their properties.
            This includes SOFTWARE and HARDWARE breakpoints and INSTRUCTION, DATA READ and DATA WRITE breakpoints.

            The following subcommands are supported:

            SUBCOMMAND, VARIANTS        DESCRIPTION
            list, ls                    List breakpoints.
            add, insert                 Add new breakpoint.
            modify, change, configure   Change breakpoint properties.
            remove, delete, rm          Remove breakpoint(s).
            enable                      Enable breakpoint(s).
            disable                     Disable breakpoint(s).
            messages                    Control display of breakpoint status
                                        messages.

            *** ALIASES

            ALIAS       COMMAND
            bp          breakpoint
            lsbp        breakpoint list

            *** NOTES

            - breakpoints can be added even if no file has been loaded.
            - the breakpoint PROPERTIES (definitions) determine the way the breakpoints will be physically PLANTED.
            - breakpoints are re-evaluated when the memory map (symbol information) of a target changes.

            - the breakpoint STATUS gives information about the physical INSTANCEs of the PLANTED breakpoints.

            - breakpoints can be associated with particular TARGETs, or can exist without target.
            - Eclipse creates breakpoints without targets, the CLI adds the current target by default.

            - breakpoint definitions are bound to the TCF client who creates them. This means breakpoints set from\
              within the Eclipse GUI and the Tcl CLI should not be modified by the other. Also they will automatically\
              be removed when the client (e.g Eclipse IDE) disconnects.

            - multiple breakpoint definitions can trigger the same point
            - a single breakpoint definition can trigger multiple points
            - to preserve resources, only the first address of a multi-point hardware breakpoint definition will be planted.

            *** EXAMPLES

            Add a software breakpoint in main.cpp file at line 50:
            % breakpoint add -type software -file main.cpp -line 50
            or:
            % bp add main.cpp:50

            Add a hardware breakpoint in main.cpp file at line 60:
            % breakpoint add -file main.cpp -line 60 -type hardware

            Display all the breakpoints:
            % breakpoint list

            Change the breakpoint with ID #1 to hardware:
            % breakpoint modify #1 -type hardware

            Delete the breakpoint with ID #2:
            % breakpoint remove #2

            Run to POSIX_Init (remove breakpoint after hit):
            % breakpoint add -temporary -location POSIX_Init; run -wait

            Easily run to line 60 of current file:
            % bp add -temp :60; contw


        } bpCommand {
            list - add - modify - remove - enable - disable - messages {
                -comment {
                    # Important commands names are declared first without argument
                    # definitions, to establish the autocomplete/help suggestion order.
                }
            }
            list - ls {
                -brief "List breakpoints."
                -args {
                    -quiet {
                        -flag
                        -brief "Return dictionary instead of printing to stdout."
                    }
                    -no-status {
                        -flag
                        -var noStatus
                        -brief "Omit status information"
                    }
                    -ids {
                        -flag
                        -var idsOnly
                        -brief "Return list of IDs."
                    }
                    args {
                        -var ids
                        -brief "Argument list of breakpoint IDs to print information about."
                        -description {
                            If none specified, all breakpoints are to be shown.
                        }
                    }
                }
                -mutex {
                    {-ids {-quiet -no-status args}}
                }
            }
            modify - change - configure {
                -brief "Change breakpoint."
                -args {
                    id {
                        -brief "ID of breakpoint to change."
                    }
                }
            }
            add - insert -
            modify - change - configure -
            remove - rm - delete -
            disable - enable {
                -args {
                    -quiet {
                        -flag
                        -brief "Hide feedback"
                        -description {
                            Temporarily disables messages about breakpoints being added/removed/changed during the call.
                            This effectively will disable feedback about the breakpoint in question.
                        }
                    }
                }
            }
            add - insert {
                -brief "Add breakpoint."
                -args {
                    -id {
                        -optional
                        -brief "ID of breakpoint."
                        -description {
                            If missing, the CLI will generate an ID automatically.
                            This ID will have the form #<number> and should be unique.
                        }
                    }
                }
            }
            add - insert - modify - change - configure {
                -args {
                    -type {
                        -optional
                        -choice {auto software hardware}
                        -var bpType
                        -brief "Breakpoint type (software, hardware, auto)."
                    }
                    -location {
                        -optional
                        -complete-script {
                            mdbg::cli::Breakpoints::ArgumentParser autocomplete-location $PARAMS $PREFIX
                        }
                        -brief "Breakpoint location."
                        -description {
                            The breakpoint location is either a symbol or an adddress.
                            Cannot be used together with "-file", "-line" and "-column".
                        }
                    }
                    -file {
                        -optional
                        -requires {-line}
                        -brief "Breakpoint file."
                        -description {
                            The file where the breakpoint is located.
                            Requires "-line".
                            Can't be used together with -location.
                        }
                    }
                    -line {
                        -optional
                        -requires {-file}
                        -brief "Breakpoint line."
                        -description {
                            The line in the file where the breakpoint is located.
                            Requires "-file".
                        }

                    }
                    -column {
                        -optional
                        -requires {-line}
                        -brief "Breakpoint column."
                        -description {
                            The column in the line in the file where the breakpoint is located.
                            Requires "-line"
                        }
                    }
                    -target {
                        -optional
                        -multi
                        -var targetList
                        -complete-script {
                            mdbg::cli::Target::getValidTargets $PREFIX*
                        }
                        -brief "Specify breakpoint target."
                        -description {
                            Affects which targets the breakpoint will be set to.
                            If missing when adding breakpoints, the current target is used.
                            The "-no-target" option can specifiy that the breakpoint is available for all the targets.
                            Can be specified multiple times or as a list of targets.
                        }
                    }
                    -no-target {
                        -flag
                        -var noTarget
                        -brief "Make breakpoint available on all targets."
                    }
                    -condition {
                        -optional
                        -var condExpr
                        -brief "Breakpoint condition."
                        -description {
                            Expression to be evaluated by TCF before reporting the target to be stopped in breakpoint.
                            If the expression evaluates to false, the framework will resume the target automatically.
                        }
                    }
                    -ignorecount {
                        -optional
                        -type integer
                        -var ignoreCount
                        -brief "Breakpoint ignore count."
                        -description {
                            Number of times the breakpoint has to be ignored.
                            That is, TCF will resume the target this number of times automatically.
                        }
                    }
                    -execute {
                        -flag
                        -brief "Instruction breakpoint."
                        -description {
                            This is the default breakpoint type.
                        }
                    }
                    -read {
                        -flag
                        -brief "Data read breakpoint."
                        -description {
                            Can be combined with "-write" and "-change".
                        }
                    }
                    -write {
                        -flag
                        -brief "Data write breakpoint."
                        -description {
                            Can be combined with "-read" and "-change".
                        }
                    }
                    -change {
                        -flag
                        -brief "Data change breakpoint."
                        -description {
                            Reserved.
                        }
                    }
                    -size {
                        -optional
                        -type integer
                        -var bpSize
                        -brief "Size of the breakpoint in bytes."
                        -description {
                            Used to specify break range.
                            NOTE: not all targets support all sizes of breakpoints.
                        }
                    }
                    -temporary {
                        -flag
                        -brief "Temporary breakpoint."
                        -description {
                            Causes the breakpoint to be deleted after first hit.
                        }
                    }
                    -skip-prologue {
                        -flag
                        -var skipPrologue
                        -brief "Skip function prologue."
                        -description {
                            Causes the execution engine to skip the function prologue when the breakpoint is being hit.
                        }
                    }
                    -stopgroup {
                        -optional
                        -var stopGroupTargetList
                        -brief "Stop other targets."
                        -description {
                            Specify the targets which need to be stopped when this breakpoint is hit.
                            The argument should be a Tcl list of target names.

                            NOTE: If the stop group does not contain the target of the breakpoint, \
                            the debugger will resume it after hitting the breakpoint.
                        }
                    }
                    -maskvalue {
                        -optional
                        -type integer
                        -var maskValue
                        -brief "Breakpoint mask value."
                        -description {
                            A breakpoint can be qualified with a mask value which may be further refined with a mask.
                        }
                    }
                    -mask {
                        -optional
                        -type integer
                        -brief "Breakpoint mask."
                        -description {
                            see "-maskvalue"
                        }
                    }
                    -inverted {
                        -flag
                        -requires {-size}
                        -brief "Invert breakpoint range."
                        -description {
                            Break if PC out of range. Not supported yet.
                        }
                    }
                    -enabled {
                        -flag
                        -brief "Enable breakpoint."
                        -description {
                            This is the default when adding new breakpoints.
                        }
                    }
                    -disabled {
                        -flag
                        -brief "Disable breakpoint."
                        -description {
                            The breakpoints added via CLI are enabled by default.
                            This switch prevents that.
                            Disables the breakpoint at change.
                        }
                    }
                }
                -mutex {
                    {-target -no-target}
                    {-location -file }
                    {-enabled -disabled}
                }
            }
            add - insert {
                -args {
                    file:line {
                        -optional
                        -regexp :[0-9]+$
                        -brief "Source location"
                        -synopsis {
                            [file]:line
                        }
                        -description {
                            This argument was added for convenience.
                            It is functionally equivalent with the parameters of the "-file" and "-line" arguments.
                            Therefore it cannot be used in conjunction with those.

                            In case the "file" part is empty, it is inferred from the current PC of the breakpoint target.
                        }

                    }
                    addressOrSymbolName {
                        -optional
                        -var location
                        -complete-script {
                            mdbg::cli::Breakpoints::ArgumentParser autocomplete-location $PARAMS $PREFIX
                        }
                        -brief "Address or symbol name."
                        -description {
                            This argument was added for convenience.
                            It is functionally equivalent with the parameter of the "-location" argument.
                            Therefore it cannot be used in conjunction with that.
                        }
                    }
                }
                -mutex {
                    {file:line addressOrSymbolName -location {-file -line}}
                }
            }
            remove - rm - delete {
                -brief "Remove breakpoint(s)."
            }
            remove - rm - delete - enable - disable {
                -args {
                    -all {
                        -flag
                        -brief "All breakpoints."
                    }
                    args {
                        -brief "Breakpoint ID(s) to consider."
                        -description {
                            Multiple IDs can to be given as consecutive arguments.
                        }
                    }
                }
            }
            modify - change - configure {
                -args {
                    -no-type        {-flag -var noBpType      -brief "Reset breakpoint type."}
                    -no-location    {-flag -var noLocation    -brief "Reset breakpoint location."}
                    -no-file        {-flag -var noFile        -brief "Reset breakpoint file."}
                    -no-line        {-flag -var noLine        -brief "Reset breakpoint line."}
                    -no-column      {-flag -var noColumn      -brief "Reset breakpoint column."}
                    -no-condition   {-flag -var noCondExpr    -brief "Reset breakpoint condition."}
                    -no-ignorecount {-flag -var noIgnoreCount -brief "Reset breakpoint ignore count."}
                    -no-access-mode {-flag -var noAccessMode  -brief "Reset breakpoint access mode."
                                                              -description {
                                                                  Resets access mode specified previously by the "-execute", "-read", "-write" or "-change" flags.
                                                              }
                    }
                    -no-size        {-flag -var noBpSize      -brief "Reset breakpoint size."}
                    -not-temporary  {-flag -var notTemporary  -brief "Make breakpoint permanent."
                                                              -description {
                                                                  Cancels the effect of the "-temporary" flag.
                                                              }
                    }
                    -dont-skip-prologue {-flag -var dontSkipPrologue -brief "Don't skip prologue"
                                                                     -description {
                                                                         Cancels the effect of "-skip-prologue" flag.
                                                                     }
                    }
                    -no-stopgroup   {-flag -var noStopGroup   -brief "Reset breakpoint stop group."}
                    -no-maskvalue   {-flag -var noMaskValue   -brief "Reset breakpoint mask value."}
                    -no-mask        {-flag -var noMask        -brief "Reset breakpoint mask."}
                    -not-inverted   {-flag -var notInverted   -brief "Reset breakpoint inverted."
                                                              -description {
                                                                  Cancels the effect of the "-inverted" flag.
                                                              }
                    }
                }
                -mutex {
                    {-type           -no-type       }
                    {-location       -no-location   }
                    {-file           -no-file       }
                    {-line           -no-line       }
                    {-column         -no-column     }
                    {-condition      -no-condition  }
                    {-ignorecount    -no-ignorecount}
                    {-size           -no-size       }
                    {-no-access-mode {-read -write -execute -change}}
                    {-temporary      -not-temporary }
                    {-skip-prologue  -dont-skip-prologue}
                    {-stopgroup      -no-stopgroup  }
                    {-maskvalue      -no-maskvalue  }
                    {-mask           -no-mask       }
                    {-inverted       -not-inverted  }
                }
            }
            enable {
                -brief "Enable breakpoint(s)."
            }
            disable {
                -brief "Disable breakpoint(s)."
            }

            messages {
                -brief "Control display of breakpoint status messages."
                -description {
                    Return the list of currently displayed event messages.
                    The following modes are supported:
                        off     - no status messages displayed
                        on      - enable essential status messages
                        verbose - all messages displayed

                        show|hide ?add|remove|change|plant|unplant|error|condition|hit?...
                                - show or hide individual message categories
                }
                -args {
                    mode {
                        -brief "Display mode."
                        -choice {off on verbose show hide}
                        -optional
                    }
                    args {
                        -requires mode
                        -choice {add remove change plant unplant error error.all condition hit}
                    }
                }
            }
            list - ls -
            modify - change - configure -
            remove - delete - rm -
            enable - disable {
                -arg-complete-scripts {
                    {id args} {
                        mdbg::shell::autocomplete::filterList [
                            ::tcf::Breakpoints::getIDs
                        ] $PREFIX*
                    }
                }
            }
        }

        set DefaultHelpText [cli::Unindent {
            *** COMMAND                                DESCRIPTION
            breakpoint {list|ls}                   List breakpoints.
            breakpoint {add|insert}                Add new breakpoint.
            breakpoint {modify|change|configure}   Change breakpoint properties.
            breakpoint {remove|delete|rm}          Remove breakpoint(s).
            breakpoint enable                      Enable breakpoint(s).
            breakpoint disable                     Disable breakpoint(s).
            breakpoint messages                    Control display of breakpoint status messages.

            !!! Type "help breakpoint" to see examples.
        }]

        oo::objdefine ArgumentParser {
            #
            #  Public
            #
            method setupBreakpointArgs {MODE {level 0}} {
                incr level
                uplevel $level [list set MODE $MODE]

                my SetupContextIdList $level
                uplevel $level {
                    if {$enabled} {
                        set disabled 0
                    } else {
                        set enabled [expr {!$disabled}]
                    }

                    if {!$temporary} {
                        unset temporary
                    }

                    if {[info exists bpType]} {
                        switch $bpType {
                            software {set bpType SOFTWARE}
                            hardware {set bpType HARDWARE}
                            default {set bpType AUTO}
                        }
                    } else {
                        if {$MODE == "ADD"} {
                            if {$read || $write} {
                                set bpType HARDWARE
                            } else {
                                set bpType SOFTWARE
                            }
                        }
                    }
                }

                my SetAccessModeFlag accessMode EXECUTE execute $level
                my SetAccessModeFlag accessMode READ read $level
                my SetAccessModeFlag accessMode WRITE write $level
                my SetAccessModeFlag accessMode CHANGE change $level

                uplevel $level {
                    if {[info exists stopGroupTargetList]} {
                        set stopGroup [::mdbg::cli::Target::getContextIdFromTargetList $stopGroupTargetList]
                    }
                }

                uplevel $level {
                    if {[info exists file:line]} {
                        regexp {^(.*):([0-9]+)$} ${file:line} -> file line
                        if {$file == ""} {
                            lassign $contextIdList fileContext
                            lassign [mdbg::state -target $fileContext -basedir -file] baseDir file
                            if {$file == ""} {
                                return -code error "no current file for $fileContext"
                            }
                            if {$baseDir != ""} {
                                set file [file join $baseDir $file]
                            }
                        }
                    }
                }

                uplevel $level {
                    set hasFile [info exists file]
                    set hasLocation [info exists location]
                }
            }

            method autocomplete-location {params prefix} {
                if {[string is integer -strict $prefix]} {
                    return [list $prefix]
                }
                if {$prefix == {} && [lindex $params end] ni {-location}} {
                    return
                }
                if {[set index [lsearch -exact $params -target]] >= 0} {
                    incr index
                    set target [lindex $params $index]
                }
                set context [mdbg::cli::Target::getContextIdFromTargetVar target]
                mdbg::cli::Symbols::getElfSymbols funcname $context $prefix*
            }

            method getBreakpoint {{BP {}} {level 0}} {
                incr level

                foreach {FIELD var noVars} {
                    ID              id               {}
                    ENABLED         enabled          {}
                    TYPE            bpType           noBpType
                    FILE            file             {hasLocation noFile}
                    LINE            line             {hasLocation noLine}
                    COLUMN          column           {hasLocation noColumn}
                    LOCATION        location         {hasFile noLocation}
                    CONTEXT_IDS     contextIdList    noTarget
                    CONDITION       condExpr         noCondExpr
                    IGNORE_COUNT    ignoreCount      noIgnoreCount
                    ACCESS_MODE     accessMode       noAccessMode
                    SIZE            bpSize           noBpSize
                    TEMPORARY       temporary        notTemporary
                    SKIP_PROLOGUE   skipPrologue     dontSkipPrologue
                    STOP_GROUP      stopGroup        noStopGroup
                    MASK_VALUE      maskValue        noMaskValue
                    MASK            mask             noMask
                } {
                    my SetBpFieldFromVar     BP $FIELD $var $level
                    my UnsetBpFieldIfFlagVars BP $FIELD $noVars $level
                }

                set BP
            } ; # oo::objdefine ArgumentParser

            #
            #  Private
            #
            method SetupContextIdList {{level 0}} {
                incr level
                uplevel $level {
                    if {[info exists contextIdList]} {
                        unset contextIdList
                    }
                    if {!$noTarget} {
                        if {[info exists targetList]} {
                            set contextIdList [mdbg::cli::Target::getContextIdFromTargetList $targetList]
                        } else {
                            if {$MODE == "ADD"} {
                                if { [info exists ::mdbg::TARGET]} {
                                    set contextIdList [list $mdbg::TARGET]
                                }
                            }
                        }
                    }
                }
            }

            method SetBpFieldFromVar {dictVar fieldName argVar level} {
                incr level
                upvar 1 $dictVar BP
                upvar $level $argVar argument
                if {[info exists argument]} {
                    dict set BP $fieldName $argument
                }
            }

            method UnsetBpFieldIfFlagVars {dictVar fieldName flagVars level} {
                if {$flagVars != {}} {
                    incr level
                    upvar 1 $dictVar BP
                    foreach flagVar $flagVars {
                        upvar $level $flagVar flag
                        if {[info exists flag] && $flag} {
                            dict unset BP $fieldName
                        }
                    }
                }
            }

            method SetAccessModeFlag {accessModeVar flagName flagVar level} {
                incr level
                upvar $level $accessModeVar accessMode
                upvar $level $flagVar flag
                if {[info exists flag] && $flag} {
                    if {![info exists accessMode]} {
                        set accessMode $flagName
                    } else {
                        lappend accessMode $flagName
                    }
                }
            }

        } ;  # oo::objdefine ArgumentParser

        proc getListOf {ids quiet {status 1} } {
            set allIDs [::tcf::Breakpoints::getIDs]
            if {![llength $allIDs]} {
                if {$quiet} {
                    return
                }
                if {![llength $ids]} {
                    cli::PutInfo "No breakpoints set."
                    return
                }
            } else {
                if {![llength $ids]} {
                    set ids $allIDs
                }
            }
            set result [dict create]
            foreach id $ids {
                if {![dict exists $result $id]} {
                    if {$id in $allIDs} {
                        dict set result $id Properties [tcf::Breakpoints::getProperties $id]
                        if {$status} {
                            dict set result $id Status [tcf::Breakpoints::getStatus $id]
                        }
                    } else {
                        set message "Breakpoint \"$id\" not defined."
                        if {$quiet} {
                            error -level 2 $message
                        } else {
                            cli::PutInfo $message
                        }
                    }
                }
            }
            return $result
        }

        cli::TcfTabularDisplay create PropertyDisplay Breakpoints::BREAKPOINT  {
            ID {
                -header "\[BPID\]"
                -present
                -space-after 4
                -wrap char
                -expand
                -transform {
                    format {[%s]} $ID ; # [mdbg::cli::Shorten $ID 70 0.3]
                }
            }
            CONTEXT_IDS {
                -present
                -header "TARGET IDs"
                -default "<any>"
                -wrap word
                -expand
            }
            TYPE {
                -header "TYPE"
                -transform {
                    switch -exact $TYPE {
                        SOFTWARE {return "\[SW\]"}
                        HARDWARE {return "\[HW\]"}
                    }
                    return $TYPE
                }
            }
            CONTEXT_NAMES {
                -skip
            }
            EXECUTABLE_PATHS {
                -skip
            }
            ACCESS_MODE {
                -header "ACCESS MODEs"
            }
            {ENABLED TEMPORARY SKIP_PROLOGUE} {
                -present
                -header "OPTIONs"
                -min-width 0
                -expand
                -wrap word
                -transform {
                    set result [list]
                    foreach {
                        var             logical  text
                    } {
                        ENABLED         false    "disabled"
                        TEMPORARY       true     "temporary"
                        SKIP_PROLOGUE   true     "skip prologue"
                    } {
                        if {[string is true -strict [set $var]] == bool($logical)} {
                            lappend result $text
                        }
                    }
                    join $result ", "
                }
            }
            LOCATION {
                -header "LOCATION"
                -min-width 9
            }
            SIZE {
                -header "SIZE"
            }
            {FILE LINE COLUMN} {
                -header "SOURCE LOCATION"
                -expand
                -wrap word
                -transform {
                    cli::FileLineColumn $FILE $LINE $COLUMN
                }
            }
            MASK_VALUE {
                -header "MASK VALUE"
                -align right
            }
            MASK {
                -header "MASK"
                -align right
            }
            TIME {
                -header "TIME"
            }
            TIME_SCALE {
                -header "TIME SCALE"
            }
            TIME_UNITS {
                -header "TIME UNITS"
            }
            CONDITION {
                -header "CONDITION"
            }
            IGNORE_COUNT {
                -header "IGN#"
                -align right
            }
            STOP_GROUP {
                -header "STOP GROUP"
            }
        }

        cli::TcfTabularDisplay create StatusDisplay Breakpoints::STATUS {
            {FILE LINE COLUMN} {
                -header "Actual source location"
                -transform {
                    cli::FileLineColumn $FILE $LINE $COLUMN
                }
            }
            ERROR {
                -header "Error"
                -expand
                -wrap word
            }
            INSTANCES {
                -skip
            }
        }

        cli::TcfTabularDisplay create InstanceDisplay Breakpoints::STATUS::INSTANCE {
            CONTEXT {
                -header "Context"
            }
            TYPE {
                -header "Type"
                -transform {
                    switch -exact $TYPE {
                        SOFTWARE {return \[SW\]}
                        HARDWARE {return \[HW\]}
                        AUTO {return Auto}
                    }
                    return $TYPE
                }
            }
            ADDRESS {
                -header "Address"
            }
            HIT_COUNT {
                -header "Hit#"
                -align right
            }
            {CONTEXT ADDRESS ERROR CONDITION_ERROR} {
                -header "Source / Error"
                -expand
                -space-before 4
                -wrap word
                -transform {
                    set result [list]
                    if {$ERROR != {}} {
                        lappend result $ERROR
                    } else {
                        if {$CONTEXT != {} && $ADDRESS != {}} {
                            lassign [cli::LineNumbers::getLineFromAddress $CONTEXT $ADDRESS] dir file line column
                            set path [cli::LineNumbers::mapFileName $dir $file]
                            set lineInfo [cli::FileLineColumn $path $line $column]
                            if {$lineInfo != {}} {
                                lappend result $lineInfo
                            }
                        }
                        if {$CONDITION_ERROR != {}} {
                            lappend result $CONDITION_ERROR
                        }
                    }
                    join $result \n
                }
            }
        } ; # create PropertyDisplay

        oo::objdefine PropertyDisplay mixin cli::ResultTable

        proc DEBUG_VAR {name} {
            upvar 1 $name var
            upvar 0 ::mdbg::Channels::debug DBGCH
            if {[info exists var]} {
                puts $DBGCH "\"$name\" is [prettyTclObject $var]"
            } else {
                puts $DBGCH "\"$name\" not set!"
            }
        }

        proc printList {status} {
            set propertyList [list]
            set statusList [list]
            set instanceList [list]
            set printOrder [list]

            set propertyIndex 0
            set statusIndex 0
            set instanceIndex 0

            dict for {id bp} $status {
                lappend printOrder doubleDivider =
                set property [dict get $bp Properties]

                lappend propertyList $property
                lappend printOrder propertyHeader *
                lappend printOrder propertyRows $propertyIndex
                incr propertyIndex

                if {[dict exists $bp Status]} {
                    set status [dict get $bp Status]

                    if {![StatusDisplay IsEmptyRow $status]} {
                        lappend statusList $status
                        lappend printOrder \
                            singleDivider - \
                            statusHeader * \
                            statusRows $statusIndex
                        incr statusIndex
                    }

                    set instances [StatusDisplay GetNamedField $status INSTANCES]

                    if {[llength $instances]} {
                        lappend printOrder \
                            singleDivider - \
                            instanceHeader *
                    }
                    foreach instance $instances {
                        lappend printOrder instanceRows $instanceIndex
                        lappend instanceList $instance
                        incr instanceIndex
                    }
                }
            }

            lassign [PropertyDisplay getTable $propertyList] propertyHeader propertyRows
            lassign [StatusDisplay getTable $statusList] statusHeader statusRows
            lassign [InstanceDisplay getTable $instanceList] instanceHeader instanceRows

            if {[llength $propertyRows]} {
                lappend printOrder doubleDivider =
            }

            set maxWidth [expr {
                max (
                    [string length $propertyHeader],
                    [string length $statusHeader],
                    [string length $instanceHeader]
                )
            }]

            set doubleDivider [string repeat "=" $maxWidth]
            set singleDivider [string repeat "-" $maxWidth]

            foreach {rows index} $printOrder {
                if {[string is integer -strict $index]} {
                    set row [lindex [set $rows] $index]
                    PropertyDisplay PutRow $row $maxWidth
                    continue
                }
                set row [set $rows]
                switch -exact $index {
                    "*" {
                        PropertyDisplay PutHeader $row $maxWidth
                    }
                    "=" {
                        PropertyDisplay PutMainDivider $row
                    }
                    "-" {
                        PropertyDisplay PutSubDivider $row
                    }
                    default {
                        PropertyDisplay PutRow $row $maxWidth
                    }
                }
            }
        }

        proc NEXT_ID {} {
            variable LAST_INDEX
            incr LAST_INDEX
            return "#$LAST_INDEX"
        }

        proc addObject {dict} {
            set ids [::tcf::Breakpoints::getIDs]
            if [dict exists $dict ID] {
                set id [dict get $dict ID]
                if {$id in $ids} {
                    return -code error "breakpoint \"$id\" already exists"
                }
            } else {
                set id [NEXT_ID]
                while {$id in $ids} {
                    set id [NEXT_ID]
                }
                dict set dict ID $id
            }
            ::tcf::Breakpoints::add::fromObject $dict
            return $id
        }

        proc getObject {id} {
            if {$id ni [::tcf::Breakpoints::getIDs]} {
                return -code error "breakpoint \"$id\" does not exist"
            }
            set Properties [tcf::Breakpoints::getProperties $id]
            tcf::fields::getNamedFieldDict Breakpoints::BREAKPOINT $Properties
        }

        proc changeObject {dict} {
            ::tcf::Breakpoints::change::fromObject $dict
        }

        proc remove {bpIdList {all 0}} {
            if {$all == 0} {
                set allIDs [::tcf::Breakpoints::getIDs]
                set removeIDs [list]
                foreach bpIdOrLoc $bpIdList {
                    if {$bpIdOrLoc in $allIDs} {
                        lappend removeIDs $bpIdOrLoc
                    } else {
                        if {[string is integer -strict $bpIdOrLoc]} {
                            # convert to decimal
                            set bpLocInt [expr {int($bpIdOrLoc)}]

                            # set up location to id dictionary of all breakpoints
                            if {![info exists locToId]} {
                                set locToId [dict create]
                                foreach bpid $allIDs {
                                    set props [tcf::Breakpoints::getProperties $bpid]
                                    # lappend allBreakpoints
                                    set location [
                                        ::tcf::fields::getNamedField Breakpoints::BREAKPOINT $props LOCATION
                                    ]

                                    if {[string is integer -strict $location]} {
                                        dict lappend locToId [expr int($location)] $bpid
                                    }
                                }
                            }
                            if {[dict exists $locToId $bpLocInt]} {
                                lappend removeIDs {*}[dict get $locToId $bpLocInt]
                                continue
                            }
                        }
                        cli::Warning "Breakpoint \"$bpIdOrLoc\" does not exist!"
                    }
                }
                ::tcf::Breakpoints::remove $removeIDs
            }
            ::tcf::Breakpoints::remove $bpIdList
        }

        proc removeAll {} {
            remove [::tcf::Breakpoints::getIDs] 1
        }

        proc enable {bpIdList {all 0}} {
            if {$all == 0} {
                set allIDs [::tcf::Breakpoints::getIDs]
                foreach bp $bpIdList {
                    if {$bp ni $allIDs} {
                        cli::Warning "Breakpoint \"$bp\" does not exist!"
                    }
                }
            }
            ::tcf::Breakpoints::enable $bpIdList
        }

        proc enableAll {} {
            enable [::tcf::Breakpoints::getIDs] 1
        }

        proc disable {bpIdList {all 0}} {
            if {$all == 0} {
                set allIDs [::tcf::Breakpoints::getIDs]
                foreach bp $bpIdList {
                    if {$bp ni $allIDs} {
                        cli::Warning "Breakpoint \"$bp\" does not exist!"
                    }
                }
            }
            ::tcf::Breakpoints::disable $bpIdList
        }

        proc disableAll {} {
            disable [::tcf::Breakpoints::getIDs] 1
        }

        if {[info object isa object ::tcf::BreakpointMonitor]} {

            oo::class create BreakpointMonitor {

                superclass ::tcf::BreakpointMonitor

                variable MaxWidth
                variable BreakpointMap

                variable DisplayModeEvents
                variable DisplayEvents

                constructor {} {
                    namespace path [list {*}[namespace path] ::mdbg]
                    set MaxWidth 75

                    set DisplayModeEvents {
                        off     {}
                        on      {add remove change plant unplant error condition}
                        verbose {add remove change plant unplant error error.all condition hit}
                    }

                    my setDisplayMode on

                    set BreakpointMap [dict create]

                    next
                }

                #
                #  Public
                #
                method setDisplayMode {mode} {
                    set DisplayEvents [dict get $DisplayModeEvents [string tolower $mode]]
                }

                method getDisplayEvents {} {
                    set DisplayEvents
                }

                method show {args} {
                    foreach event $args {
                        if {$event ni $DisplayEvents} {
                            lappend DisplayEvents $event
                        }
                    }
                }
                method hide {args} {
                    set DisplayEvents [lmap event $DisplayEvents {
                        if {$event in $args} {
                            continue
                        }
                        set event
                    }]
                }

                #
                #  Private
                #
                method IsImportantError {error} {
                    foreach glob {
                        "*Invalid expression"
                        "*Invalid address"
                        "*Unresolved source line information"
                    } {
                        if {[string match $glob $error]} {
                            return 0
                        }
                    }
                    return 1
                }

                method PutInstanceError {Mode context error type address} {
                    set prefix [my EventPrefix $context]
                    if {$address == {}} {
                        cli::Put$Mode "$prefix: Breakpoint error ($error)"
                    } else {
                        set bpstr [my BpStr $type $context $address]
                        cli::Put$Mode "$prefix: Could not plant $bpstr: $error"
                    }
                }

                #
                #  Virtual/Overrides
                #
                method OnInstanceError {instance} {
                    my CheckEventDisplay error error.all
                    lassign [my InstanceFieldList $instance {
                        CONTEXT ERROR TYPE ADDRESS
                    }]  context error type address

                    if {[my IsImportantError $error]} {
                        set Mode Warning
                    } else {
                        my CheckEventDisplay error.all
                        set Mode Info
                    }
                    my PutInstanceError $Mode $context $error $type $address
                }

                method OnNoInstancePlantedError {bpid instance} {
                    my CheckEventDisplay error
                    lassign [my InstanceFieldList $instance {
                        CONTEXT ERROR TYPE ADDRESS
                    }]  context error type address

                    if {[my IsImportantError $error] || [my IsEventDisplayed error.all]} {
                        return
                    }

                    set prefix [my EventPrefix $context]
                    my PutInstanceError Info $context $error $type $address
                }

                method EventPrefix {context} {
                    cli::Target::ctxId2target $context $context
                }

                method OnInstanceCondError {instance} {
                    my CheckEventDisplay condition
                    lassign [my InstanceFieldList $instance {
                        CONTEXT TYPE ADDRESS CONDITION_ERROR
                    }]  context type address error

                    set prefix [my EventPrefix $context]
                    set bpstr [my BpStr $type $context $address]
                    cli::PutWarning "$prefix: Error evaluating conditon for $bpStr ($error)"
                }

                method OnInstanceHit {instance} {
                    my CheckEventDisplay hit
                    lassign [my InstanceFieldList $instance {
                        CONTEXT TYPE ADDRESS HIT_COUNT
                    }]  context type address count

                    set prefix [my EventPrefix $context]
                    set bpstr [my BpStr $type $context $address]

                    cli::PutEvent "$prefix: Hit $bpstr (Hit count: $count)"
                }

                method OnInstancePlanted {instance} {
                    my CheckEventDisplay plant
                    lassign [my InstanceFieldList $instance {
                        CONTEXT TYPE ADDRESS
                    }]  context type address

                    set prefix [my EventPrefix $context]
                    set bpstr [my BpStr $type $context $address]

                    cli::PutInfo "$prefix: Planted $bpstr"
                }

                method OnInstanceUnplanted {instance} {
                    my CheckEventDisplay unplant
                    lassign [my InstanceFieldList $instance {
                        CONTEXT TYPE ADDRESS
                    }]  context type address

                    set prefix [my EventPrefix $context]
                    set bpstr [my BpStr $type $context $address "from"]

                    cli::PutInfo "$prefix: Unplanted $bpstr"
                }

                method OnAdded {event bpList} {
                    next $event $bpList
                    foreach bp $bpList {
                        my MapBreakpoint $bp
                    }
                    my CheckEventDisplay add
                    my ShowBreakpoints add $bpList
                }

                method OnChanged {event bpList} {
                    next $event $bpList
                    foreach bp $bpList {
                        my MapBreakpoint $bp
                    }
                    my CheckEventDisplay change
                    my ShowBreakpoints change $bpList
                }

                method OnRemoved {event bpIdList} {
                    next $event $bpIdList
                    set bpList [list]
                    foreach bpid $bpIdList {
                        my UnmapBreakpoint $bpid bpList
                    }
                    my CheckEventDisplay remove
                    my ShowBreakpoints remove $bpList
                }

                #
                #  Private
                #
                method CheckEventDisplay {event args} {
                    if {![my IsEventDisplayed $event {*}$args]} {
                        return -level 2
                    }
                }

                method IsEventDisplayed {event args} {
                    foreach event [list $event {*}$args] {
                        if {$event in $DisplayEvents} {
                            return 1
                        }
                    }
                    return 0
                }


                method GetDisplayLevel {mode} {
                    set index [lsearch -nocase $DisplayModes $mode]
                    if {$index < 0} {
                        return -code error "invalid mode \"$mode\", must be: $DisplayModes"
                    }
                    return $index
                }

                method BpStr {type context address {for for}} {
                    set result [my BpType $type]
                    if {$context != ""} {
                        append result " $for [cli::Target::getBreakpointContextName $context]"
                    }
                    if {[string is integer -strict $address]} {
                        append result " at $address"

                        lassign [cli::LineNumbers::getLineFromAddress \
                            $context $address] dir file line column

                        if {$file != {}} {
                            append result " (file: $file"
                            if {$line != {}} {
                                append result ", line $line"
                            }
                            if {$column != {}} {
                                append result ", column $column"
                            }
                            append result ")"
                        } else {
                            set symName [sym at \
                                -addr $address \
                                -context $context \
                                -quiet -first \
                                -nocomplain \
                            ]
                            if {$symName != ""} {
                                append result " <$symName>"
                            }
                        }
                    }

                    return $result
                }

                method MapBreakpoint {bpDict} {
                    set bpid [::tcf::getField $bpDict ID]
                    if {$bpid != ""} {
                        dict set BreakpointMap $bpid $bpDict
                    }
                }

                method UnmapBreakpoint {bpid bpListVar} {
                    upvar 1 $bpListVar bpList
                    dict update BreakpointMap $bpid bp {
                        if {[info exists bp]} {
                            lappend bpList $bp
                            unset bp
                        }
                    }
                }

                method BpType {type} {
                    switch -glob -nocase $type {
                        soft*   { return "software breakpoint" }
                        hard*   { return "hardware breakpoint" }
                        auto*   { return "automatic breakpoint" }
                        ""      { return "breakpoint" }
                        default { return "`<$type>` breakpoint" }
                    }
                }

                method ShowBreakpoints {event bpList} {
                    set config {
                        add    { "added"     PutResult      }
                        change { "changed"   PutResult      }
                        remove { "removed"   Puts           }
                    }
                    lassign [dict get $config $event] eventStr putRow

                    lassign [cli::Breakpoints::PropertyDisplay getTable $bpList] header rows
                    set bpstr [expr {[llength $bpList] == 1 ? "Breakpoint" : "Breakpoints"}]
                    set width [string length $header]

                    cli::PutInfo [cli::AlignCenter " $bpstr $eventStr " $width "="]
                    cli::Puts $header
                    foreach row $rows {
                        cli::$putRow $row
                    }
                    cli::PutInfo [string repeat "=" $width]
                }
            } ; # oo::class create BreakpointMonitor

            BreakpointMonitor create Monitor

            dll::atexit {
                Monitor shutdown
            }

        } ; # if { [info object isa object ::tcf::BreakpointMonitor] }

    } ; # namespace eval cli::Breakpoints

    proc breakpoint {args} {
        if {![llength $args]} {
            cli::Help::PutLong $::mdbg::cli::Breakpoints::DefaultHelpText
            return
        }
        cli::NoErrorStack {
            cli::Breakpoints::ArgumentParser parseCallerArgs 1 $args

            if {$bpCommand in {
                add insert
                modify change configure
                remove rm delete
                disable enable}
            } {
                if {$quiet} {
                    set hiddenEvents [
                        lsearch -all -inline -regexp [
                            cli::Breakpoints::Monitor getDisplayEvents
                        ] {^(add|remove|change)$}
                    ]
                    if {[llength hiddenEvents]} {
                        cli::Breakpoints::Monitor hide {*}$hiddenEvents
                    } else {
                        unset hiddenEvents
                    }
                }
            }
            try {
                switch -exact $bpCommand {
                    list - ls {
                        if {$idsOnly} {
                            return [::tcf::Breakpoints::getIDs]
                        }
                        set bpList [cli::Breakpoints::getListOf $ids $quiet [expr !$noStatus]]
                        if {$quiet} {
                            return $bpList
                        }
                        cli::Breakpoints::printList $bpList
                    }
                    add - insert {
                        cli::Breakpoints::ArgumentParser setupBreakpointArgs ADD
                        set BP [cli::Breakpoints::ArgumentParser getBreakpoint]
                        cli::Breakpoints::addObject $BP
                    }
                    modify - change - configure {
                        set IsAddingBP no
                        set BP [cli::Breakpoints::getObject $id]
                        cli::Breakpoints::ArgumentParser setupBreakpointArgs MODIFY
                        set BP [cli::Breakpoints::ArgumentParser getBreakpoint $BP]
                        cli::Breakpoints::changeObject $BP
                    }
                    remove - delete - rm  {
                        if {$all} {
                            cli::Breakpoints::removeAll
                        } else {
                            cli::Breakpoints::remove $args
                        }
                    }
                    enable {
                        if {$all} {
                            cli::Breakpoints::enableAll
                        } else {
                            cli::Breakpoints::enable $args
                        }
                    }
                    disable {
                        if {$all} {
                            cli::Breakpoints::disableAll
                        } else {
                            cli::Breakpoints::disable $args
                        }
                    }
                    messages {
                        if {[info exists mode]} {
                            switch $mode {
                                show {
                                    cli::Breakpoints::Monitor show {*}$args
                                }
                                hide {
                                    cli::Breakpoints::Monitor hide {*}$args
                                }
                                default {
                                    cli::Breakpoints::Monitor setDisplayMode $mode
                                }
                            }
                            cli::Breakpoints::Monitor getDisplayEvents
                        }
                    }
                    default {
                        cli::NotImplemented
                    }
                }
            } finally {
                if {[info exists hiddenEvents]} {
                    update
                    cli::Breakpoints::Monitor show {*}$hiddenEvents
                }
            }
        }
    }

    shell::autocomplete::addCommand {breakpoint breakpoints} mdbg::cli::Breakpoints::ArgumentParser autocomplete
    cli::Help::Manager add breakpoint -parser mdbg::cli::Breakpoints::ArgumentParser

    proc breakpoints {args} {
        cli::DeprecatedWarning "breakpoint"
        breakpoint {*}$args
    }

    cli::Alias {bp} breakpoint
    cli::Alias lsbp breakpoint list

    namespace export breakpoint

} ; # namespace eval mdbg
