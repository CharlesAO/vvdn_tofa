# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : TabularDisplay
# Description: Tabular data display helper classes
# Created on : Sep 6, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************


namespace eval mdbg::cli {

    #
    #   Display lists of dictionaries in a tabular format
    #
    oo::class create TabularDisplay {

        variable FieldDefs
        variable CreateNS

        constructor {fieldDefs {namespace {}} } {
            set FILL { }
            my SetupNS $namespace

            # Column definition parser
            ::mdbg::cli::ArgumentParser create FieldDefParser {
                -auto
                -args {
                    -present {-flag}
                    -skip {-flag}
                    -header {-optional}
                    -expr {-optional}
                    -format {-optional}
                    -transform {-optional}
                    -align {-choice {left right center} -default left}
                    -max-width {-optional -type integer -var maxWidth}
                    -expand {-flag}
                    -min-width {-optional -type integer -var minWidth}
                    -wrap {-choice {none char word} -default {none} -requires {-max-width -expand} }
                    -space-before {-default 2 -type integer -var spaceBefore}
                    -space-after {-default 2 -type integer -var spaceAfter}
                    -default {-default {}}
                    -tab {-optional -type integer}
                }
                -mutex {
                    {{-space-before -expand} -tab}
                    {-max-width -expand}
                    {-transform {-expr -format}}
                }
            }

            foreach {field defs} $fieldDefs {
                my AddFieldDef $field $defs
            }
        }

        method display {objlist} {
            my Inline_FillTable
            my Inline_GetTable
            if {$TableWidth && [llength $TableRows]} {
                my Inline_PrintTable
            }
        }

        method getTable {objlist} {
            my Inline_FillTable
            my Inline_GetTable
            list $TableHeader $TableRows
        }

        #
        #  Protected
        #

        method FILL {} {
            return " "
        }

        method GetNamedField {obj fieldName default} {
            if {![dict exists $obj $fieldName]} {
                return $default
            }
            dict get $obj $fieldName
        }
        export GetNamedField

        method IsEmptyRow {obj} {

            dict for {fieldNameList fieldDefs} $FieldDefs {
                if {[dict get $fieldDefs skip]} {
                    continue
                }
                set defaults [dict get $fieldDefs default]

                set fieldCount [llength $fieldNameList]
                my Inline_GetFieldValues

                if {!$EmptyFieldValues} {
                    return 0
                }
            }
            return 1
        }
        export IsEmptyRow

        method Puts {string} {
            tailcall ::mdbg::cli::Puts $string
        }
        export Puts

        method SetupNS {namespace} {
            if {![info exists CreateNS]} {
                if {$namespace != ""} {
                    set CreateNS $namespace
                } else {
                    set CreateNS [uplevel 2 namespace current]
                }
            }
        }

        #
        #  Private
        #

        method AddFieldDef {field defs} {
            set defs [FieldDefParser parse "$field \{" $defs "\}"]
            dict set FieldDefs $field $defs
        }

        #
        #  "Inline" methods, operating on caller's stack
        #

        method Inline_FillTable {} {
            uplevel 1 {
                set table [list]
                set columnList [list]
                set totalWidth 0
                set expandColumns [list]
                set prevColumnDef [dict create]
                dict for {fieldNameList fieldDefs} $FieldDefs {
                    if {![dict get $fieldDefs skip]} {
                        my Inline_FillColumn
                    }
                }

                if {[my AllowExpand] && [llength $expandColumns]} {
                    my Inline_ExpandColumns
                }
            }
        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method AllowExpand {} {
            expr 1
        }

        method Inline_ExpandColumns {} {
            uplevel 1 {
                set totalFixedWidth $totalWidth
                set totalNegotiableWidth 0
                set totalFieldWidthSum 0
                set totalPreferredWidth 0
                foreach column $expandColumns {
                    lassign $column index fixed preferred sum
                    set column
                    incr totalFixedWidth [expr {$fixed - $preferred}]
                    incr totalNegotiableWidth [expr {$preferred - $fixed}]
                    incr totalFieldWidthSum $sum
                    incr totalPreferredWidth $preferred
                }
                set availableNegotiableWidth [expr {[::mdbg::cli::ConsoleWidth] - (1 + $totalFixedWidth) }]
                if {    $availableNegotiableWidth > 0
                     && $availableNegotiableWidth < $totalNegotiableWidth
                } {
                    foreach column $expandColumns {
                        lassign $column col fixed preferred sum
                        set columnDef [lindex $columnList $col]
                        dict update columnDef Width width maxWidth max wrap wrap {
                            set width [expr {
                                $fixed + int((
                                    .25 * $preferred  / $totalPreferredWidth +
                                    .75 * $sum / $totalFieldWidthSum
                                ) * $availableNegotiableWidth)
                            }]
                            if {$width < $max} {
                                set max $width
                                set needWrap [expr {$wrap != "none"}]
                            } else {
                                set needWrap 0
                            }
                        }

                        lset columnList $col $columnDef
                        if {$needWrap} {
                            set columnData [list]
                            foreach fieldValue [lindex $table $col] {
                                set fieldLines [my GetFieldLines $fieldValue $columnDef]
                                lappend columnData [join $fieldLines \n]
                            }
                            lset table $col $columnData
                        }
                    }
                }
            }
        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method Inline_FillColumn {} {
            uplevel 1 {
                set columnDef $fieldDefs
                if {![dict exists $columnDef header]} {
                    dict set columnDef header $fieldNameList
                }

                set headerWidth [string length [dict get $columnDef header]]

                my Inline_FillColumnData

                if {[dict get $columnDef present]} {
                    my Inline_AdjustColumnWidth
                    my Inline_SetAlignFunction
                    my Inline_AddColumnToTable
                }
            }
        }

        method GetFieldLines {fieldValue columnDef} {

            set wrap [dict get $columnDef wrap]
            if {![dict exists $columnDef maxWidth]} {
                set maxWidth 0
            } else {
                set maxWidth [dict get $columnDef maxWidth]
            }

            if {$maxWidth <= 0 || $wrap ni {word char}} {
                return [split $fieldValue \n]
            }

            set WrapFun [dict get {
                word ::mdbg::cli::WordWrap
                char ::mdbg::cli::CharWrap
            } $wrap]

            $WrapFun $fieldValue $maxWidth
        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method Inline_FillColumnData {} {
            uplevel 1 {
                set maxFieldWidth 0
                set sumOfFieldWidths 0
                set columnData [list]
                set defaults [dict get $columnDef default]
                set fieldCount [llength $fieldNameList]

                foreach obj $objlist {
                    set fieldValueList [list]

                    my Inline_GetFieldValues

                    if {$EmptyFieldValues && ![dict get $columnDef present]} {
                        lappend columnData {}
                        continue
                    }

                    set fieldLines [my GetFieldLines $fieldValue $columnDef]
                    set fieldValue [join $fieldLines \n]

                    dict set columnDef present 1
                    lappend columnData $fieldValue

                    set fieldWidth 0

                    foreach fieldLine $fieldLines {
                        set lineWidth [string length $fieldLine]
                        if {$lineWidth > $fieldWidth} {
                            set fieldWidth $lineWidth
                            if {$fieldWidth > $maxFieldWidth} {
                                set maxFieldWidth $fieldWidth
                            }
                        }
                    }

                    incr sumOfFieldWidths $fieldWidth
                }
            }
        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method Inline_GetFieldValues {} {
            uplevel 1 {
                set EmptyFieldValues 1
                for {set fieldIndex 0} {$fieldIndex < $fieldCount} {incr fieldIndex} {
                    set fieldName [lindex $fieldNameList $fieldIndex]
                    set default [lindex $defaults $fieldIndex]
                    set fieldValue [my GetNamedField $obj $fieldName $default]
                    if {$fieldValue != {}} {
                        set EmptyFieldValues 0
                    }
                    lappend fieldValueList $fieldValue
                }

                if {!$EmptyFieldValues} {
                    if {[dict exists $fieldDefs transform]} {
                        set transform [dict get $fieldDefs transform]
                        set fieldValue [
                            apply [list $fieldNameList $transform $CreateNS] {*}$fieldValueList
                        ]
                    } elseif {[dict exists $fieldDefs expr]} {
                        set expr [dict get $fieldDefs expr]
                        set fieldValue [
                            apply [list $fieldNameList [list expr $expr] $CreateNS] {*}$fieldValueList
                        ]
                        if {[dict exists $fieldDefs format]} {
                            set format [dict get $fieldDefs format]
                            set fieldValue [format $format $fieldValue]
                        }
                    } elseif {[dict exists $fieldDefs format]} {
                            set format [dict get $fieldDefs format]
                            set fieldValue [format $format {*}$fieldValueList]
                    } else {
                        set fieldValue [join $fieldValueList]
                    }
                    set EmptyFieldValues [expr {
                        [string trim $fieldValue] == ""
                    }]
                }
            }
        }

        method Inline_AddColumnToTable {} {
            uplevel 1 {
                lappend columnList $columnDef
                lappend table $columnData
            }
        }

        method Inline_AdjustColumnWidth {} {
            uplevel 1 {
                if {[dict exists $columnDef minWidth]} {
                    set minWidth [dict get $columnDef minWidth]
                    set maxFieldWidth [expr {
                        max( $minWidth, $maxFieldWidth )
                    }]
                } else {
                    set minWidth $headerWidth
                    set maxFieldWidth [expr {
                        max( $headerWidth, $maxFieldWidth )
                    }]
                }
                if {[dict exists $columnDef maxWidth]} {
                    set maxFieldWidth [expr {
                        min([dict get $columnDef maxWidth],$maxFieldWidth)
                    }]
                }
                if {$headerWidth > $maxFieldWidth} {
                    dict set columnDef header [
                        string range [
                            dict get $columnDef header
                        ] 0 [
                            expr {$maxFieldWidth - 1}
                        ]
                    ]
                }
                dict set columnDef Width $maxFieldWidth
                if {$maxFieldWidth > 0} {

                    incr totalWidth [my SpaceCount prevColumnDef $columnDef $totalWidth]
                    incr totalWidth $maxFieldWidth

                    if {[dict get $columnDef expand]} {
                        lappend expandColumns [list [llength $table] $minWidth $maxFieldWidth $sumOfFieldWidths]
                        dict set columnDef maxWidth $maxFieldWidth
                    }
                }
            }
        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method Inline_SetAlignFunction {} {
            uplevel 1 {
                switch -exact [dict get $columnDef align] {
                    right {
                        set align ::mdbg::cli::AlignRight
                    }
                    center {
                        set align ::mdbg::cli::AlignCenter
                    }
                    left -
                    default {
                        set align ::mdbg::cli::AlignLeft
                    }
                }
                dict set columnDef AlignFunction $align
            }
        }

        method Inline_GetTable {} {
            uplevel 1 {
                set numCols [llength $table]
                set numRows [llength $objlist]
                set TableHeader [my Inline_GetHeader]
                set TableRows [my Inline_GetRows]
                set TableWidth [string length $TableHeader]
            }
        }

        method Inline_GetHeader {} {
            uplevel 1 {
                set line {} ;  # important variable name
                set prevColumnDef [dict create]
                for {set col 0} {$col < $numCols} {incr col} {
                    set columnDef [lindex $columnList $col]
                    my AddSpace line prevColumnDef $columnDef
                    append line [[dict get $columnDef AlignFunction] [dict get $columnDef header] [dict get $columnDef Width]]
                }
                set line
            }
        }

        method Inline_GetMultiLineRow {} {
            uplevel 1 {
                set maxRows 0
                set multiLineRow [list]
                for {set col 0} {$col < $numCols} {incr col} {
                    set data [lindex $table $col $row]
                    set multiLineData [split $data \n]
                    for {set lidx 0} {$lidx < [llength $multiLineData]} {incr lidx} {
                        if {$lidx < [llength $multiLineRow]} {
                            set line [lindex $multiLineRow $lidx]
                        } else {
                            set line [lrepeat $numCols {}]
                        }
                        lset line $col [lindex $multiLineData $lidx]
                        lset multiLineRow $lidx $line
                    }
                }
            }
        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method Inline_GetRows {} {
            uplevel 1 {
                set rows [list]
                for {set row 0} {$row < $numRows} {incr row} {
                    my Inline_GetMultiLineRow
                    set lines {}
                    foreach singleRow $multiLineRow {
                        set line {}
                        set prevColumnDef [dict create]
                        for {set col 0} {$col < $numCols} {incr col} {
                            set columnDef [lindex $columnList $col]
                            set data [lindex $singleRow $col]
                            my AddSpace line prevColumnDef $columnDef
                            set AlignFunction [dict get $columnDef AlignFunction]
                            set width [dict get $columnDef Width]
                            append line [$AlignFunction $data $width]
                        }
                        lappend lines $line
                    }
                    lappend rows [join $lines \n]
                }
                set rows
            }
        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method SpaceCount {prevColumnDefVar nextColumnDef lineLength} {
            upvar 1 $prevColumnDefVar prevColumnDef
            set spaceCount 0
            if {[dict exists $nextColumnDef tab]} {
                set tab [dict get $nextColumnDef tab]
                set spaceBeforeNext 0
            } else {
                set spaceBeforeNext [dict get $nextColumnDef spaceBefore]
                set tab 0
            }
            if {[dict size $prevColumnDef]} {
                set spaceAfterPrev [dict get $prevColumnDef spaceAfter]
                if {[dict get $prevColumnDef expand]} {
                    set tab 0
                }
            } else {
                set spaceAfterPrev 0
                set tab 0
                set spaceBeforeNext 0
            }
            set prevColumnDef $nextColumnDef
            expr {
                $tab
                    ? $tab - $lineLength % $tab
                    : max ( $spaceAfterPrev, $spaceBeforeNext )
            }

        }

    } ; # oo::class create TabularDisplay

    oo::define TabularDisplay {

        method AddSpace {lineVar prevColumnDefVar columnDef} {
            upvar 1 $lineVar line
            upvar 1 $prevColumnDefVar prevColumnDef
            set lineLength [string length $line]
            set spaceBefore [my SpaceCount prevColumnDef $columnDef $lineLength]
            if {$spaceBefore > 0} {
                append line [string repeat [my FILL] $spaceBefore]
            }
            return
        }

        method Inline_PrintTable {} {
            uplevel 1 {
                my Puts $TableHeader
                foreach tableRow $TableRows {
                    my Puts $tableRow
                }
            }
        }

    } ; # oo::class TabularDisplay

    oo::define TabularDisplay {

        variable Title

        method SetTitle {value} {
            set Title $value
        }

        method UnsetTitle {value} {
            unset -nocomplain Title
        }

    } ; # oo::class TabularDisplay

    # Mixin class for color display + trim
    oo::class create ListResultTable {
        method PutHeader {header {maxWidth 0}} {
            ::mdbg::cli::PutColor {bold} [string trimright $header]
        }
        export PutHeader

        method PutRow {row {maxWidth 0}} {
            foreach row [split $row \n] {
                my FillRight row $maxWidth
                ::mdbg::cli::Puts [string trimright $row]
            }
        }
        export PutRow

        method PutInactiveRow {row {maxWidth 0}} {
            foreach row [split $row \n] {
                my FillRight row $maxWidth
                ::mdbg::cli::PutEvent [string trimright $row]
            }
        }
        export PutInactiveRow

        method PutMainDivider {divider} {
            ::mdbg::cli::PutInfo [string trimright $divider]
        }
        export PutMainDivider

        method PutSubDivider {subdiv} {
            ::mdbg::cli::PutEvent [string trimright $subdiv]
        }
        export PutSubDivider

        #
        #  Private
        #
        method FillRight {textVar maxWidth} {
            if {int( $maxWidth ) > 0} {
                upvar 1 $textVar text
                set fill [expr {int ($maxWidth) - [string length $text]}]
                if {$fill > 0} {
                    append text [string repeat [my FILL] $fill]
                }
            }
        }

        method Inline_PrintTable {} {
            uplevel 1 {
                my PutHeader $TableHeader
                foreach row $TableRows {
                    my PutRow $row
                }
            }
        }
    } ; # oo::class

    # Mixin class for color display
    oo::class create ResultTable {
        variable ColorScheme
        variable MonochromeChangedEventId

        method GetColor {name} {
            if {![info exists ColorScheme]} {
                set ColorScheme [dict create]
                my SubscribeToMonochromeChanged
                set isMono [::mdbg::cli::IsFlagVarTrue ::mdbg::MONOCHROME]
                set isBlack [expr {![::mdbg::cli::IsEclipseTerminal white]}]
                foreach {
                    NAME            black               white               mono
                } {
                    BACK            blue                cyan                default
                    HEADER          {bold yellow}       {bold white}        bold
                    ROW             {bold default}      {bold default}      default
                    INACTIVE_ROW    {white}             {white}             {black bold}
                    MAIN_DIVIDER    {cyan}              {bold black}        {bold black}
                    SUB_DIVIDER     {black}             {white}             {black bold}
                } {
                    dict set ColorScheme $NAME [expr {$isMono ? $mono : $isBlack ? $black : $white}]
                }
            }
            list {*}[dict get $ColorScheme $name] [dict get $ColorScheme BACK]
        }

        method ResetColorScheme {} {
            unset ColorScheme
            ::mdbg::cli::MonochromeChanged cancel $MonochromeChangedEventId
            unset MonochromeChangedEventId
        }

        method SubscribeToMonochromeChanged {} {
            if {[info exists MonochromeChangedEventId]} {
                return
            }
            set MonochromeChangedEventId [::mdbg::cli::MonochromeChanged bind {my ResetColorScheme}]
        }

        method PutColor {name text} {
            set color [::mdbg::cli::RemapColor [my GetColor $name]]
            ::mdbg::cli::PutColor $color $text
        }

        method PutHeader {header {maxWidth 0}} {
            my FillRight header $maxWidth
            my PutColor HEADER $header
        }
        export PutHeader

        method PutRow {row {maxWidth 0}} {
            foreach row [split $row \n] {
                my FillRight row $maxWidth
                my PutColor ROW $row
            }
        }
        export PutRow

        method PutInactiveRow {row {maxWidth 0}} {
            foreach row [split $row \n] {
                my FillRight row $maxWidth
                my PutColor INACTIVE_ROW $row
            }
        }
        export PutInactiveRow

        method PutMainDivider {divider} {
            my PutColor MAIN_DIVIDER $divider
        }
        export PutMainDivider

        method PutSubDivider {subdiv} {
            my PutColor SUB_DIVIDER $subdiv
        }
        export PutSubDivider

        #
        #  Private
        #
        method FillRight {textVar maxWidth} {
            if {int( $maxWidth ) > 0} {
                upvar 1 $textVar text
                set fill [expr {int ($maxWidth) - [string length $text]}]
                if {$fill > 0} {
                    append text [string repeat [my FILL] $fill]
                }
            }
        }

        method Inline_PrintTable {} {
            uplevel 1 {
                set fdiv [string repeat = $TableWidth]
                if {[info exists Title] && $Title != ""} {
                    set hdiv [mdbg::cli::AlignCenter " $Title " $TableWidth = ]
                } else {
                    set hdiv $fdiv
                }

                # set subDiv [string repeat - $TableWidth]
                my PutMainDivider $hdiv
                my PutHeader $TableHeader
                # my PutSubDivider $subDiv
                foreach row $TableRows {
                    my PutRow $row
                }
                my PutMainDivider $fdiv
            }
        }
    } ; # oo::class ResultTable

    #
    #   Display for list of TCF-returned JSON dictionaries
    #
    oo::class create TcfTabularDisplay {

        superclass ::mdbg::cli::TabularDisplay

        variable ObjectType

        constructor {objectType definitions {namespace {}} } {
            my SetupNS $namespace
            next $definitions
            set ObjectType $objectType
        }

        # Override
        method GetNamedField {obj fieldName {default {}} } {
            if {$fieldName == "*"} {
                ::tcf::fields::getNamedFieldDict $ObjectType $obj
            } else {
                ::tcf::fields::getNamedField $ObjectType $obj $fieldName $default
            }
        }
        export GetNamedField

    } ; # oo::class TcfTabularDisplay

    #
    #  Displays for list of lists
    #
    oo::class create GridDisplay {

        superclass ::mdbg::cli::TabularDisplay

        constructor {definitions {namespace {}} } {
            my SetupNS $namespace
            next $definitions
        }

        # Override
        method GetNamedField {list index {default {}} } {
            if {$index < 0 || [llength $list] <= $index } {
                return $default
            }
            lindex $list $index
        }
        export GetNamedField

    } ; # oo::class GridDisplay

} ; # namespace eval mdbg::cli
