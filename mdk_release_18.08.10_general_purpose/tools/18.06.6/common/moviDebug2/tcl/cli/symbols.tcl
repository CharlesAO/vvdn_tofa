# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/symbols.tcl
# Description: symbols command + related
# Created on : Aug 19, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::Symbols {

        namespace path ::mdbg

        cli::MultiCmdArgParser create ArgumentParser -brief "Retrieve symbol information." -description {
            List or query information related to symbols.

            *** ALIASES

            ALIAS       COMMAND
            symbol      sym
            symaddr     sym addr
            locals      sym locals -quiet

            *** EXAMPLES

            List ELF symbols starting with the letter "m", with details:
            % sym list -full m*

            Get the address of "main":
            % sym addr main

            Display a table of local variables (and their values):
            % sym locals

            Get the name of the current function + offset:
            % sym at -addr [state -pc] -quiet -first
            or:
            % state -sym

        } -auto symCmd {
            list - locals - addr - at - inrange {
                -comment {
                    # This is just to establish help/completion order
                }
            }
            list - locals - inrange - at {
                -args {
                    -quiet {
                        -flag
                        -brief "Don't print result, just return value."
                    }
                }
            }
            at {
                -args {
                    -addr {
                        -optional
                        -var address
                        -brief "Address"
                        -description {
                            Specify address.
                            Cannot be used together with last `address` argument
                        }
                    }
                }
            }
            list - inrange - at - addr - locals - size - exists {
                -args {
                    -target {
                        -optional
                        -brief "Select target."
                        -description {
                            Default is current target.
                            Cannot be used with "-context".
                        }
                        -complete-script {
                            mdbg::cli::Target::getValidTargets $PREFIX*
                        }
                    }
                    -context {
                        -optional
                        -var contextId
                        -brief "Target context ID."
                        -description {
                            Same as "-target" but always expects canonical context identifier.
                            Cannot be used with "-target".
                        }
                        -hidden
                    }
                }
                -mutex {
                    {-context -target}
                }
            }
            list {
                -brief "List symbols."
                -description {
                    List ELF symbols matching optional pattern.
                }
                -args {
                    -full {
                        -flag
                        -brief "Full symbol information."
                    }
                    pattern {
                        -default *
                        -brief "Filter pattern."
                    }
                }
            }
            locals {
                -brief "Get local variables"
                -args {
                    -frame {
                        -brief "Select frame."
                        -description {
                            Default is top frame (i.e. 0).
                        }
                        -default 0
                        -type integer
                    }
                }
            }
            addr {
                -brief "Get address of symbol."
            }
            exists {
                -brief "Return logical true if symbol exists."
            }
            size {
                -brief "Get size of symbol."
            }

            addr - at - size {
                -args {
                    -nocomplain {
                        -flag
                        -brief "Don't throw error when symbol is not found"
                        -var noComplain
                    }
                }
            }

            addr - size - exists {
                -args {
                    name {
                    }
                }
            }
            at {
                -brief "Get symbol(s) at address."
                -args {
                    -first {
                        -flag
                        -requires {-quiet}
                        -brief "Only return first matching symbol name."
                    }
                    address {
                        -type integer
                        -optional
                    }
                }
                -mutex {
                    -addr address
                }
                -requires {
                    {-addr address}
                }
            }
            inrange {
                -brief "List all symbols in address range."
                -args {
                    low {-type integer}
                    high {-type integer}
                }
            }
        }

        cli::TcfTabularDisplay create Display Symbols::SYMBOL {
            ADDRESS {}
            NAME {
                -expand
                -wrap char
            }
            SIZE {
                -align right
                -transform {
                    expr {$SIZE != 0 ? $SIZE : ""}
                }
            }
            {TYPE_CLASS CLASS} {
                -header {CLASS}
                -transform {
                    if {$TYPE_CLASS == {} || $TYPE_CLASS == $CLASS} {
                       return $CLASS
                    }
                    return "$TYPE_CLASS / $CLASS"
                }
            }
            LENGTH {}
            LOWER_BOUND {}
            UPPER_BOUND {}
            OFFSET {}
            VALUE {}
            REGISTER {}

            ID {-skip}
            OWNER_ID {-skip}
            UPDATE_POLICY {-skip}
            TYPE_ID {-skip}
            BASE_TYPE_ID {-skip}
            INDEX_TYPE_ID {-skip}
            CONTAINER_ID {-skip}
            BIG_ENDIAN {-skip}
            FLAGS {-skip}
            BIT_STRIDE {}
            BINARY_SCALE {}
            DECIMAL_SCALE {}
        }

        oo::objdefine Display {
            mixin cli::ResultTable
        }

        oo::objdefine ArgumentParser {

            method setupArgs { {level 0} } {
                incr level
                uplevel $level {
                    if {![info exists contextId]} {
                        set contextId [cli::Target::getContextIdFromTargetVar target]
                    }

                }
            }

        }

        proc listElfSymbols {mode contextId {pattern *}} {
            Display display [getElfSymbols $mode $contextId $pattern]
        }

        proc getElfSymbols {mode contextId {pattern *}} {
            set result [list]
            set count 0

            set ModeDict {
                id {
                    lappend result $id
                }
                name {
                    lappend result $name
                }
                addr+name {
                    lappend result $addr $name
                }
                id+addr+name {
                    lappend result $id $addr $name
                }
                funcname {
                    set symbol [tcf::Symbols::getContext $id]
                    if {[tcf::getSymbolField $symbol CLASS] == {FUNCTION}} {
                        lappend result $name
                    }
                }
                row {
                    lappend result $row
                }
                brief {
                    lappend result [dict create \
                        $ID $id $NAME $name $ADDRESS $addr]
                }
                full {
                    lappend result [tcf::Symbols::getContext $id]
                }
            }
            if {![dict exists $ModeDict $mode]} {
                return code -error "invalid mode \"$mode\": must be one of: [dict keys $ModeDict]"
            }
            if {$mode == {brief}} {
                foreach keyVar {ID NAME ADDRESS} {
                    set $keyVar [set ::tcf::Symbols::SYMBOL::FIELDS::${keyVar}]
                }
            }
            set body [dict get $ModeDict $mode]
            foreach fileName [getLoadedElfFileNames $contextId] {
                foreach row [tcf::getElfSymbols $contextId $fileName $pattern] {
                    lassign $row id name addr
                    if {$addr == {} || $addr == 0} {
                        continue
                    }
                    uplevel 0 $body
                }
            }
            return $result
        }

        proc getLoadedElfFileNames {id} {
            set fileNames [list]
            foreach contextID [tcf::queryContext [format {ID="%s"/**} $id]] {
                foreach fileName [tcf::getMemoryMapTable $contextID {FILE_NAME}] {
                    if {$fileName ni $fileNames} {
                        lappend fileNames $fileName
                    }
                }
            }
            set fileNames
        }

        proc getLocalVars {mode contextId {pattern *}} {
            set ModeDict {
                id {
                    lappend result $id
                }
                name {
                    lappend result $name
                }
                addr+name {
                    lappend result $addr $name
                }
                id+addr+name {
                    lappend result $id $addr $name
                }
                full {
                    lappend result $sym
                }
            }
            if {![dict exists $ModeDict $mode]} {
                return code -error "invalid mode \"$mode\": must be one of: [dict keys $ModeDict]"
            }

            set filterName [expr {$pattern != "*"}]
            set needFields [expr {
                $filterName || ( $mode in {name addr+name id+addr+name})
            }]
            set needSym [expr {$needFields || ( $mode in {full} )}]

            set body [dict get $ModeDict $mode]

            set result [list]
            foreach id [tcf::Symbols::listStackSymbols $contextId] {
                if {$needSym} {
                    set sym [tcf::Symbols::getContext $id]
                    if {$needFields} {
                        lassign [tcf::getSymbolFieldList $sym {
                            ADDRESS NAME
                        }]  addr    name
                        if {$filterName} {
                            if {![string match $pattern $name]} {
                                continue
                            }
                        }
                    }
                }
                uplevel 0 $body
            }
            set result
        }

        proc getMemRanges {contextID} {
            ::tcf::getMemoryMapTable $contextID {ADDR SIZE}
        }

        proc indexInRanges {rangeList address} {
            set index -1
            if {$address != {}} {
                foreach range $rangeList {
                    incr index
                    lassign $range rAddr rSize
                    if {$rAddr == "" || $rSize == ""} {
                        continue
                    }
                    if { $rAddr <= $address && $address < $rAddr + $rSize } {
                        return $index
                    }
                }
            }
            return -1
        }

        proc IntListCompare {lhs rhs} {
            foreach a $lhs b $rhs {
                set diff [expr {wide($a) - wide($b)}]
                if {$diff} {
                    return $diff
                }
            }
            return 0;
        }

        proc findByName {contextID symName {pc 0}} {
            set symIDs [::tcf::Symbols::findByName $contextID $pc $symName]
            set memrgn {}
            switch -exact [llength $symIDs] {
                0 {
                    set id [::tcf::Symbols::find $contextID $pc $symName]
                    ::tcf::Symbols::getContext $id
                }
                1 {
                    ::tcf::Symbols::getContext [lindex $symIDs 0]
                }
                default {
                    if {[::tcf::isFrameContextId $contextID]} {
                        set contextID [::tcf::getFrameContextParentId $contextID]
                    }

                    set memr [getMemRanges $contextID]
                    # {index notype addr} -> sym
                    set symbols [dict create]
                    set pos -1
                    foreach id $symIDs {
                        incr pos
                        set sym [::tcf::Symbols::getContext $id]
                        set addr [tcf::getSymbolField $sym ADDRESS]
                        if {$addr != {}} {
                            set index [indexInRanges $memr $addr]
                            if {$index < 0} {
                                set index [llength $memr]
                            }
                        } else {
                            set addr [expr {wide(1) << 32}]
                            set index [llength $memr] + 1
                        }

                        set noType [expr {[tcf::getSymbolField $sym TYPE_ID] == {}}]
                        if {$index < 0} {
                            set index
                        }
                        set key [list $index $noType $addr $pos]
                        dict set symbols $key $sym
                    }
                    set sortedKeys [lsort -command IntListCompare [dict keys $symbols]]
                    set firstKey [lindex $sortedKeys 0]
                    return [dict get $symbols $firstKey]
                }
            }
        }

        proc forEachIdInContextRange {symIdVarName contextID begin end body} {
            upvar 1 $symIdVarName symId
            upvar 1 SKIP_SIZE SKIP_SIZE
            for {set addr $begin} {$addr < $end} {incr addr $delta} {
                set delta 1
                set max [expr $end - $addr]
                if {[catch {
                            set ids [tcf::Symbols::findByAddr $contextID $addr]
                        } message]} {
                    continue
                }
                foreach symId $ids {
                    uplevel 1 $body
                    if {[info exists SKIP_SIZE]} {
                        if {$SKIP_SIZE > 0 && $delta < $SKIP_SIZE} {
                            set delta $SKIP_SIZE
                        }
                        unset SKIP_SIZE
                    }
                }
            }
        }

        proc getAddress {symNameOrAddress} {
            if {[string is integer -strict $symNameOrAddress]} {
                set symNameOrAddress
            } elseif {[isRegDefined $symNameOrAddress]} {
                getRegAddr $symNameOrAddress
            } else {
                sym addr $symNameOrAddress
            }
        }

        proc getSymSize {symName} {
            sym size $symName
        }

    } ; # namespace eval cli::Symbols

    proc sym {args} {
        if {![llength $args]} {
            help sym *
            return
        }
        cli::Symbols::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {
            cli::Symbols::ArgumentParser setupArgs
            switch -exact $symCmd {
                list {
                    set mode [expr {
                        $full ? "full" : "brief" }
                    ]
                    if {$quiet} {
                        cli::Symbols::getElfSymbols $mode $contextId $pattern
                    } else {
                        cli::Symbols::listElfSymbols $mode $contextId $pattern
                    }
                }
                addr {
                    if {[catch {
                         cli::Symbols::findByName $contextId $name
                    } sym options]} {
                        if {$noComplain} {
                            return
                        }
                        return -options $options $sym
                    }
                    set addr [tcf::getSymbolField $sym ADDRESS]
                    return $addr
                }
                exists {
                    if {[catch {
                        cli::Symbols::findByName $contextId $name
                    } sym]} {
                        return 0
                    }
                    return [
                        expr {[dict size $sym] > 0}
                    ]
                }
                size {
                    set sym [cli::Symbols::findByName $contextId $name]
                    if {[catch {
                        tcf::getSymbolField $sym SIZE 0
                    } size options]} {
                        if {$noComplain} {
                            return
                        }
                        return -options $options $size
                    }
                    return $size
                }
                at {
                    ::tcf::fields::Using Symbols::SYMBOL NAME ADDRESS
                    if {[catch {
                        tcf::Symbols::findByAddr $contextId $address
                    } symbolIDs options]} {
                        if {$noComplain} {
                            return
                        }
                        return -options $options $symbolIDs
                    }
                    set result [list]
                    foreach symID $symbolIDs {
                        set symObj [tcf::Symbols::getContext $symID]
                        lassign [tcf::getSymbolFieldList $symObj {NAME ADDRESS}] symName symAddr
                        set delta [expr $address - $symAddr]
                        if {$delta} {
                            set symName "$symName+[cli::hexstr $delta]"
                        }
                        if {$first} {
                            return $symName
                        }
                        lappend result [
                            dict create \
                                $ADDRESS $address \
                                $NAME $symName \
                        ]
                    }

                    if {$quiet} {
                        return $result
                    }
                    cli::Symbols::Display display $result
                }
                inrange {
                    cli::TrapInterrupt {
                        set lastAddr {}
                        set result [list]

                        cli::Symbols::forEachIdInContextRange symID $contextId $low $high {
                            cli::CheckInterrupt

                            set symObj [tcf::Symbols::getContext $symID]

                            lassign [tcf::getSymbolFieldList $symObj {
                                ADDRESS SIZE
                            } {
                                0 1
                            }] address SKIP_SIZE
                            if {$address != $lastAddr} {
                                lappend result $symObj
                                set lastAddr $address
                            }
                        }
                        if {$quiet} {
                            return $result
                        }
                        cli::Symbols::Display display $result
                    }
                }
                locals {
                    set contextId [cli::Target::getContextIdFromTargetVar target]
                    if {![info exists target]} {
                        set target [cli::Target:::getTargetNameFromContextId $contextId]
                    }
                    set frameId [::tcf::getStackFrameId $contextId $frame]
                    if {$frameId == {}} {
                        return
                    }
                    if {$quiet} {
                        return [cli::Symbols::getLocalVars name $frameId]
                    }
                    set vars [list]
                    set ids [cli::Symbols::getLocalVars id $frameId]
                    foreach id $ids {
                        if {[catch {
                            lappend vars [mget -quiet -target $target -id $id]
                        } error options]} {
                            cli::Warning "$id: $error"
                        }
                    }
                    cli::Memory::displaySymInfoItemList $vars
                }
                default {
                    cli::NotImplemented 1
                }
            }
        }
    }

    shell::autocomplete::addCommand sym cli::Symbols::ArgumentParser autocomplete
    cli::Help::Manager add sym -parser cli::Symbols::ArgumentParser

    cli::Alias symbol sym
    cli::Alias symaddr sym addr
    cli::Alias locals sym locals -quiet

    namespace export sym

} ; # namespace eval mdbg

