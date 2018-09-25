# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : linenumbers.tcl
# Description: Line number information
# Created on : Nov 24, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::LineNumbers {

        namespace path ::mdbg

        cli::ArgumentParser create ArgumentParser {
            -brief "Show line number information at address or source location"
            -description {
                Shows line number information as present in .debug_lines section.

                Either an address or a source file/line combination should be provided.

                *** ALIASES

                ALIAS      FULL FORM
                linfo      linenumbers
                lnum       linenumbers

                *** EXAMPLES

                Show line number debug information for the current instruction
                % linenumbers

                Show line number debug information for the address 0x70181328:
                % linenumbers -addr 0x70181328

                Show line number debug information for the return address
                % linenumbers -frame 1

                Show line number debug information for the current instruction of Shave 0
                % linenumbers -target S0

                Show line number debug information for line 42 of main.cpp:
                % linenumbers -file main.cpp -line 42

                Return the address mapped to line 42 of main.cpp:
                % linenumbers -file main.cpp -line 42 -value

                List all known C++ source files having lines mapped to Shave 0:
                % linenumbers -target S0 -list-files *.cpp

            }
            -auto
            -args {
                -show-source {
                    -flag
                    -synopsis "?-s[how-source]?"
                    -var showSource
                    -brief "List actual source lines if possible."
                }
                -s { -flag -hidden -var showSource}
                -addr {
                    -brief "Start address"
                    -description {
                        Retrieve line number information at address.
                    }
                    -type integer
                    -optional
                }
                -size {
                    -brief "Size of code range at address"
                    -description {
                        Retrieve line number information for all addresses in
                        range, starting from "-addr", sized "-size".
                        Default is 1.
                    }
                    -type integer
                    -default 1
                    -requires {-addr}
                }
                -file {
                    -brief "File name"
                    -description {
                        Retrieve line number information for file.
                        Requires "-line" to be specified as well.

                        Cannot be used together with "-addr".
                    }
                    -optional
                    -requires {-line}
                }
                -line {
                    -brief "Line number"
                    -description {
                        Retrieve line number information at line.
                        Requires "-file" to be specified as well.

                        Cannot be used together with "-addr".
                    }
                    -optional
                    -requires {-file}
                }
                -list-files {
                    -brief "Retrieve list of source files"
                    -description {
                        `pattern(*)` is a `glob`-style pattern to filter file names. e.g "*.c"

                        Only one of the `pattern (*)` arguments can be given at a time.

                        Cannot be used together with "-file", "-addr", "-s[how-source]", "-value" and "-display"
                    }
                    -flag
                    -var listFiles
                    -opt-param pattern(1)
                    -opt-var fileNamePattern
                }
                -no-dirs {
                    -requires {-list-files}
                    -flag -var noDirs
                    -brief "Omit base directory from listing"
                    -description {
                        Requires `-list-files`.

                        Causes to display or return only source file names, not their base directories.
                    }
                }
                -column {
                    -brief "Column."
                    -description {
                        Retrieve line number information for this column.
                    }
                    -default 0
                    -requires {-line}
                }
                -value {
                    -brief "Do not display. Return first value as result."
                    -description {
                        Returns either a numeric address, or a {filepath line} pair.
                    }
                    -flag
                }
                -quiet {
                    -brief "Do not display. Return TCF structure."
                    -flag
                }
                -target {
                    -brief "Select target for operation"
                    -description {
                        Default is current target.
                    }
                    -optional
                    -complete-script {
                        cli::Target::getValidTargets $PREFIX*
                    }
                }
                -frame {
                    -brief "Select stack frame for operation"
                    -description {
                        Use current program counter on specified frame as address.
                        If missing, the top frame (actual current PC) is used.

                        Cannot be used together with "-addr", "-file" or "-list-files".
                    }
                    -type integer
                    -optional
                }
                -display {
                    -brief "When \"-value\" requested, also do a full display."
                    -description {
                        This is the default behaviour, except when "-value" is also specified.
                    }
                    -flag
                }
                pattern(2) {
                    -optional
                    -var fileNamePattern
                    -requires {-list-files}
                    -brief "File name pattern for list-files."
                    -description {
                        Provided for convenience. Requires `-list-files`.

                        Only one of the `pattern(*)` arguments can be given at a time.
                    }
                }
            }
            -mutex {
                {-addr -file -list-files -frame}
                {-list-files {-s -show-source -value -display}}
                {-s -show-source}
                {-quiet {-value -display} }
                {pattern(1) pattern(2)}
            }
        }

        cli::TcfTabularDisplay create Display LineNumbers::CodeArea  {
            {START_ADDRESS} {
                -header "ADDRESS"
                -min-width 10
                -present
            }
            {START_ADDRESS END_ADDRESS} {
                -header "SIZE"
                -align right
                -min-width 4
                -expr {
                    $END_ADDRESS-$START_ADDRESS
                }
            }
            {DIRECTORY FILE} {
                -header "FILE"
                -present
                -expand
                -wrap word
                -transform {
                    file join $DIRECTORY $FILE
                }
            }
            {START_LINE END_LINE} {
                -header "LINE"
                -present
                -transform {
                    incr END_LINE -1
                    if {$START_LINE == $END_LINE} {
                        return $START_LINE
                    }
                    return "$START_LINE - $END_LINE"
                }
            }
            ISA {
            }
            {IS_STMT BASIC_BLOCK PROLOGUE_END EPILOGUE_BEGIN} {
                -header "Extra information"
                -expand
                -wrap word
                -transform {
                    set result [list]
                    if {[string is true -strict $IS_STMT]} {
                        lappend result "<statement>"
                    }
                    if {[string is true -strict $BASIC_BLOCK]} {
                        lappend result "<basic block>"
                    }
                    if {[string is true -strict $PROLOGUE_END]} {
                        lappend result "<prologue end>"
                    }
                    if {[string is true -strict $EPILOGUE_BEGIN]} {
                        lappend result "<epilogue begin>"
                    }
                    join $result " "
                }
            }
        }

        oo::objdefine Display {
            mixin cli::ListResultTable
        }

        oo::class create SourceTableDisplay {
            superclass cli::GridDisplay

            variable CurrentIndex
            variable RowIndex
            variable Marker

            constructor {} {
                next {
                    0 {
                        -header LINE
                        -align right
                        -space-after 1
                    }
                    2 {
                        -header ""
                        -min-width 0
                        -space-before 0
                        -space-after 0
                    }
                    1 {
                        -header SOURCE
                        -align left
                        -expand
                        -wrap word
                        -transform {
                            cli::expandTabs ${1}
                        }
                    }
                }
                set Marker "*"
            }

            method display {lines {current {}} } {
                unset -nocomplain CurrentIndex
                set RowIndex 0
                if {$current != "" && [llength $lines] > 1} {
                    set index 0
                    foreach line $lines {
                        lassign $line number src
                        if {$number == $current} {
                            lappend line Marker
                            set CurrentIndex $index
                            lset lines $index 2 $Marker
                            break
                        }
                        incr index
                    }
                }
                next $lines
            }

            method PutRow {row} {
                set PutFunc Puts
                if {[info exists CurrentIndex] && $CurrentIndex == $RowIndex} {
                    set PutFunc PutResult
                }
                incr RowIndex
                foreach row [split $row \n] {
                    ::mdbg::cli::${PutFunc} [string trimright $row]
                }
            }

            method PutHeader {header} {
                ::mdbg::cli::PutResult [string trimright $header]
            }
            method Inline_PrintTable {} {
                uplevel 1 {
                    my PutHeader $TableHeader
                    foreach row $TableRows {
                        my PutRow $row
                    }
                }
            }
        }

        SourceTableDisplay create SourceDisplay

        cli::GridDisplay create SourceFileTable {
            1 {
                -header "FILE PATH"
            }
            0 {
                -header "BASE DIRECTORY"
                -wrap word
                -expand
            }
        }
        oo::objdefine SourceFileTable mixin cli::ResultTable

        proc fromAddr {contextId startAddress endAddress} {
            tcf::LineNumbers::mapToSource $contextId $startAddress $endAddress
        }

        proc fromSource {contextId file line column} {
            tcf::LineNumbers::mapToMemory $contextId $file $line $column
        }

        proc fromPC {contextId {pc {}}} {
            if {$pc == {}} {
                set pc [tcf::getContextPC $contextId]
            }
            set npc [expr $pc + 1]
            fromAddr $contextId $pc $npc
        }

        proc fillCodeAreaList {codeAreaListVar showSource onlyFirst} {
            ::tcf::fields::Using LineNumbers::CodeArea DIRECTORY

            upvar 1 $codeAreaListVar codeAreaList
            cli::UpdateList codeAreaList codeArea {
                if {[dict exists $codeArea $DIRECTORY]} {
                    set prevDir [dict get $codeArea $DIRECTORY]
                } else {
                    if {[info exists prevDir]} {
                        dict set codeArea $DIRECTORY $prevDir
                    }
                }

                if {$showSource} {
                    cli::LineNumbers::readSourceLines codeArea
                }
                if {$onlyFirst} {
                    break
                }
            }

        }

        proc displayTable {codeAreaList} {
            lassign [Display \
                getTable $codeAreaList
            ] mainHeader mainRows

            set needHeader 1

            set len [llength $codeAreaList]
            for {set i 0} {$i < $len} {incr i} {
                if {$needHeader} {
                    cli::PutResult $mainHeader
                    set needHeader 0
                }
                foreach row [split [lindex $mainRows $i] \n] {
                    cli::Puts [string trimright $row]
                }
                set codeArea [lindex $codeAreaList $i]
                if {[dict exists $codeArea SOURCE_LINES]} {
                    set sourceLines [dict get $codeArea SOURCE_LINES]
                    SourceDisplay display $sourceLines
                    set needHeader 1
                } elseif {[dict exists $codeArea SOURCE_LINES_ERROR]} {
                    set error [dict get $codeArea SOURCE_LINES_ERROR]
                    cli::PutWarning $error
                    set needHeader 1
                }
            }
        }

        proc getSourceValue {codeArea} {
            lassign [tcf::getCodeAreaFieldList $codeArea {
                DIRECTORY FILE START_LINE SOURCE_LINES SOURCE_LINES_ERROR
            } ] dir       file line       src          error

            set result [list [file join $dir $file] $line]

            if {[llength $src]} {
                lappend result [lindex $src 0 1]
            } elseif {$error != {}} {
                lappend result {} $error
            }
            set result
        }

        proc getAddressValue {codeArea} {
            tcf::getCodeAreaField $codeArea START_ADDRESS
        }

        proc readSourceLines {codeAreaVar} {
            upvar 1 $codeAreaVar codeArea
            lassign [tcf::getCodeAreaFieldList $codeArea {
                DIRECTORY FILE START_LINE END_LINE
            }]  dir       file start      end

            set scriptBody {
                dict set codeArea \
                    SOURCE_LINES [getFileLines $dir $file $start $end]
            }
            if {[catch $scriptBody error options]} {
                if {[dict exists $options -errorcode]} {
                    set errorCode [dict get $options -errorcode]
                    dict set codeArea SOURCE_LINES_ERROR_CODE $error
                }
                dict set codeArea SOURCE_LINES_ERROR $error
            }
            return
        }

        proc mapFileName {dir fileName} {
            if {$::tcl_platform(platform) == "windows"} {
                if {[regexp {^/cygdrive/([a-zA-Z])/(.*)$} $dir -> drive path]} {
                    set dir $drive:/$path
                }
            }
            file join $dir $fileName
        }

        proc getFileLines {dir fileName start end} {
            set fd [open [mapFileName $dir $fileName] r]
            try {
                set lineNum 1
                if {$start != {}} {
                    while {$lineNum < $start && ![eof $fd]} {
                        gets $fd
                        incr lineNum
                    }
                }
                set resultLines [list]
                while {( $end == {} || $lineNum < $end ) && ![eof $fd]} {
                    set lineText [gets $fd]
                    lappend resultLines [list $lineNum $lineText]
                    incr lineNum
                }
                return $resultLines
            } finally {
                close $fd
            }
        }

        proc showSourceLine {id pc} {
            set codeAreaList [cli::LineNumbers::fromPC $id $pc]
            if {[llength $codeAreaList]} {
                set codeArea [lindex $codeAreaList 0]
                readSourceLines codeArea
                lassign [cli::LineNumbers::getSourceValue $codeArea] file line source error
                cli::PutfInfo {%s:%d} $file $line
                if {$source != {}} {
                    cli::Puts [cli::expandTabs $source]
                    return
                }
                if {$error != {}} {
                    cli::PutWarning $error
                }
            }
            cli::Dasm::showDasmLine $id $pc
        }

        # returns {dir file line column}
        proc getLineFromAddress {context address} {
            set lineInfoList [fromAddr $context $address [expr {$address + 1}]]
            set firstItem [lindex $lineInfoList 0]
            ::tcf::getCodeAreaFieldList $firstItem {DIRECTORY FILE START_LINE START_COLUMN}
        }

    } ; # namespace eval cli::LineNumbers

    proc linenumbers {args} {
        cli::LineNumbers::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {
            set id [cli::Target::getContextIdFromTargetVar target]

            if {$listFiles} {
                set xargs [list]
                if {[info exists fileNamePattern]} {
                    lappend xargs $fileNamePattern
                }
                set result [tcf::getSourceFileList $id {*}$xargs]
                if {$noDirs} {
                    set result [lmap item $result {lindex $item 1}]
                }
                if {$quiet} {
                    return $result
                }
                if {[llength $result]} {
                    if {$noDirs} {
                        cli::PutResult "FILE NAME"
                        foreach file $result {
                            cli::Puts $file
                        }
                    } else {
                        cli::LineNumbers::SourceFileTable display $result
                    }
                } else {
                    set message "No source files found for target \"$id\""
                    if {[info exists fileNamePattern]} {
                        append message " matching pattern \"$fileNamePattern\"."
                    }
                    cli::PutInfo $message
                }
                return
            }

            set result [
                if {[info exists addr]} {
                    set ValueProc getSourceValue
                    cli::LineNumbers::fromAddr $id $addr [expr {$addr + $size}]
                } elseif {[info exists file]} {
                    if {$value && $showSource && !$display} {
                        set display 1
                    }
                    set ValueProc getAddressValue
                    cli::LineNumbers::fromSource $id $file $line $column
                } else {
                    set pc {}
                    if {[info exists frame]} {
                        set frameId [cli::CallStack::getFrameId $id $frame]
                        set pc [tcf::getContextPC $frameId]
                    }
                    set ValueProc getSourceValue
                    cli::LineNumbers::fromPC $id $pc
                }
            ]

            if {!$value} {
                set display 1
            }
            cli::LineNumbers::fillCodeAreaList result $showSource [expr {!$display}]
            if {$quiet} {
                return [tcf::getCodeAreaDictList $result]
            }

            if {$result == {}} {
                if {$display} {
                    cli::PutInfo "No line information found."
                }
                return
            }

            if {$display} {
                cli::LineNumbers::displayTable $result
            }
            if {$value} {
                cli::LineNumbers::${ValueProc} [lindex $result 0]
            }
        }
    }

    shell::autocomplete::addCommand linenumbers cli::LineNumbers::ArgumentParser autocomplete
    cli::Help::Manager add linenumbers -parser cli::LineNumbers::ArgumentParser

    cli::ArgumentParser create cli::ListSource::ArgumentParser {
        -brief "List source code."
        -description {
            Display source code listing for the specified file
            or the current target's current source file.

            *** ALIASES

            ALIAS   FULL FORM
            lsrc    listsource

            *** EXAMPLES

            List the source code between line 20 and 40 from current source file:
            % listsource 20 40
            % listsource -start 20 -end 40

            List the code for the current line and the next 5 lines:
            % listsource +5
            % listsource -end +5

            List the code for the 5 lines before, and the current source line:
            % listsource -start -5
            % listsource -5

            List the code for the 2 lines before, the current line, and the next 3 lines:
            % listsource -start -2 -end +2
            % listsource -2 +2

            List the all the source code for the file:
            % listsource -file d:/mdk/examples/HowTo/Cpp/cppshave/leon/main.cpp
            % listsource d:/mdk/examples/HowTo/Cpp/cppshave/leon/main.cpp

            List the source code for lines 10 to 40 of file:
            % listsource -start 10 -end 40 -file leon/main.cpp
            % listsource -start 10 -end 40 leon/main.cpp
            % listsource -file leon/main.cpp -start 10 -end 40
            % listsource -file leon/main.cpp 10 40
            % listsource leon/main.cpp 10 40

            List the source code from line 20 to the end of file:
            % listsource -file leon/main.cpp -start 20 -end end
            % listsource leon/main.cpp 20 end
        }
        -args {
            -target {
                -brief "Select target for operation"
                -description {
                    Default is current target.
                    Cannot be used with `fileName`.
                }
                -optional
                -complete-script {
                    cli::Target::getValidTargets $PREFIX*
                }
            }
            -frame {
                -brief "Select stack frame for operation"
                -description {
                    Use current program counter on specified frame as address.
                    If missing, the top frame (actual current PC) is used.

                    Cannot be used with `fileName`.
                }
                -type integer
                -optional
            }
            -file {
                -var fileName
                -optional
                -brief ""
                -description {
                    See `fileName` below.
                }
            }
            -start {
                -var startLine
                -brief ""
                -description {
                    See `[-]startLine` below.
                }
                -optional
                -type integer
            }
            -end {
                -var endLine
                -brief ""
                -description {
                    See `[-]endLine` below.
                }
                -optional
                -regexp {^end|[\+\-]?[0-9]*$}
            }
            fileName {
                -optional
                -brief "Source file name."
                -description {
                    If missing, will use current target's current source file.
                }
                -not-regexp {^[\+\-]?[0-9]*$}
                -not-match {end}
            }
            startLine {
                -brief "Index of first line."
                -synopsis {
                    ?[-]startLine?
                }
                -description {
                    It can be determined automatically for the current source file.
                    The default is 1.

                    A "-" prefix makes it a value relative to current source line.
                }
                -optional
                -regexp {^[^\+]}
                -not-match {end}
                -type integer
            }
            endLine {
                -synopsis {
                    ?[+/-]endLine?
                }
                -brief "Index of last line (abs/rel)."
                -regexp {^end|[\+\-]?[0-9]*$}
                -optional
                -description {
                    A "+" or "-" prefix makes it a relative number.

                    "end" is treated specially and it means the last line in the file.

                    It can be determined automatically for the current source file.
                    The default is +0 when fileName is not present.
                }
            }
        }
        -mutex {
            {{-target -frame} -file fileName}
            {-start startLine}
            {-end endLine }
        }
    }

    proc listsource {args} {
        cli::ListSource::ArgumentParser parseCallerArgs 1 $args

        cli::NoErrorStack {
            set autoFileName [expr {![info exists fileName]}]

            if {$autoFileName} {
                set xargs [list]
                if {[info exists target]} {
                    lappend xargs -target $target
                }
                if {[info exists frame]} {
                    lappend xargs -frame $frame
                }
                set lines [linenumbers -quiet {*}$xargs]
                if {![llength $lines]} {
                    cli::PutWarning "No line information."
                    return
                }
                set line [lindex $lines 0]

                set dir [dict get $line DIRECTORY]
                set file [dict get $line FILE]
                set fileName [cli::LineNumbers::mapFileName $dir $file]

                set currentLine [dict get $line START_LINE]
                set startLineInfo $currentLine
                set endLineInfo $currentLine

                if {[info exists startLine]} {
                    if {[regexp {^[\+\-]} $startLine]} {
                        incr startLineInfo $startLine
                        unset startLine
                    }
                }

                if {[info exists endLine]} {
                    if {[regexp {^[\+\-]} $endLine]} {
                        if {[info exists startLine]} {
                            set endLine [expr {$startLine + $endLine}]
                        } else {
                            incr endLineInfo $endLine
                            unset endLine
                        }
                    } elseif {$endLine != "end"} {
                        if {![info exists startLine]} {
                            incr endLineInfo $endLine
                            unset endLine
                        }
                    }
                } else {
                    if {[info exists startLine]} {
                        set endLine $startLine
                    }
                }

                if {![info exists startLine]} {
                    set startLine $startLineInfo
                }
                if {![info exists endLine]} {
                    set endLine $endLineInfo
                }
            } else {
                set currentLine ""
                if {![info exists startLine]} {
                    set startLine 1
                    if {![info exists endLine]} {
                        set endLine end
                    }
                }
                if {[info exists endLine]} {
                    if {[regexp {^[\+\-]} $endLine]} {
                        if {[info exists startLine]} {
                            set endLine [expr {$startLine + $endLine}]
                        }
                    }
                } else {
                    set endLine $startLine
                }
            }

            if {$endLine == "end"} {
                set endLine {}
            } else {
                if {$endLine < $startLine} {
                    lassign [list $endLine $startLine] startLine endLine
                }
                incr endLine
            }

            if {[catch {
                set lines [cli::LineNumbers::getFileLines {} $fileName $startLine $endLine]
                if {$autoFileName} {
                    cli::PutResult FILE
                    cli::Puts $fileName
                }
                if {[llength $lines]} {
                    cli::LineNumbers::SourceDisplay display $lines $currentLine
                } else {
                    cli::PutInfo "No lines for $startLine-$endLine."
                }
            } error]} {
                cli::PutWarning "No lines for [file tail $fileName]:$startLine-$endLine."
                cli::PutWarning $error
            }
        }
    }

    shell::autocomplete::addCommand listsource  cli::ListSource::ArgumentParser autocomplete
    cli::Help::Manager add listsource -parser cli::ListSource::ArgumentParser

    cli::Alias {linfo lnum} linenumbers
    cli::Alias {lsrc} listsource

    namespace export linenumbers listsource

} ; # namespace eval mdbg