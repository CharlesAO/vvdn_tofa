# ***************************************************************************
# Copyright (C) 2017 Movidius Ltd. All rights reserved
# ***************************************************************************
namespace eval mdbg::shell::bricks {

    proc Start {} {
        package require Tk

        variable Top
        variable Canvas

        catch {wm withdraw .}

        set Top .bricks!

        toplevel $Top
        set Canvas [tk::canvas $Top.canvas -background black]

        wm title $Top "Bricks!"
        wm resizable $Top 0 0

        pack $Canvas

        variable Dark
        set Dark #101010

        variable Colors
        set Colors [list $Dark red orange yellow lime green cyan blue magenta]

        variable Rows
        variable Columns
        variable ScaleX
        variable ScaleY

        set Rows 20
        set Columns 10
        set ScaleX 32
        set ScaleY 32

        $Canvas configure -width [expr {$Columns * $ScaleX}] -height [expr {$Rows * $ScaleY}]

        bind $Canvas <Destroy> [list namespace inscope [namespace current] {
            unset -nocomplain Freeze
            unset -nocomplain Speed
            unset -nocomplain LightShow
            unset -nocomplain Canvas
            unset -nocomplain Top
        }]

        bind $Top <Key-Escape> [list destroy $Top]

        variable LightShow

        set LightShow 0
        CreateGrid

        NewGame
    }

    proc CreateGrid {} {
        variable Canvas
        variable Tiles
        variable Rows
        variable Columns
        variable ScaleX
        variable ScaleY
        variable Dark

        set Tiles [list]
        for {set row 0} {$row < $Rows} {incr row} {
            set tileRow [list]
            for {set col 0} {$col < $Columns} {incr col} {
                set dim [list \
                    [expr {$ScaleX * $col}] [expr {$ScaleY * $row}] \
                    [expr {$ScaleX * ( $col + 1 ) - 1}] [expr {$ScaleY * ( $row + 1 )- 1}]
                ]
                lappend tileRow [$Canvas create rectangle {*}$dim -fill $Dark]
            }
            lappend Tiles $tileRow
        }

    }

    proc RandomColorIndex {} {
        variable Colors
        expr {int(rand() * ([llength $Colors] - 1)) + 1}
    }

    proc EnableKeys {} {
        variable Top
        variable Freeze
        set Freeze 0
        if {[info exists Top]} {
            bind $Top <Key-Left>  [namespace current]::MoveLeft
            bind $Top <Key-Right> [namespace current]::MoveRight
            bind $Top <Key-Down>  [namespace current]::MoveDown
            bind $Top <Key-Up>    [namespace current]::Rotate
        }
    }

    proc DisableKeys {} {
        variable Top
        variable Freeze
        set Freeze 1
        if {[info exists Top]} {
            bind $Top <Key-Left> {}
            bind $Top <Key-Right> {}
            bind $Top <Key-Down> {}
            bind $Top <Key-Up>   {}
        }
    }

    proc Wait {ms} {
        after $ms incr dummy
        vwait dummy
    }

    variable Bricks {
        [] {
            {0 0 0 0}
            {0 1 1 0}
            {0 1 1 0}
            {0 0 0 0}
        }
        I1 {
            {0 1 0 0}
            {0 1 0 0}
            {0 1 0 0}
            {0 1 0 0}
        }
        I2 {
            {0 0 0 0}
            {1 1 1 1}
            {0 0 0 0}
            {0 0 0 0}
        }
        Z1 {
            {0 0 0 0}
            {1 1 0 0}
            {0 1 1 0}
            {0 0 0 0}
        }
        Z2 {
            {0 1 0 0}
            {1 1 0 0}
            {1 0 0 0}
            {0 0 0 0}
        }
        S1 {
            {0 0 0 0}
            {0 1 1 0}
            {1 1 0 0}
            {0 0 0 0}
        }
        S2 {
            {0 1 0 0}
            {0 1 1 0}
            {0 0 1 0}
            {0 0 0 0}
        }
        L1 {
            {0 1 0 0}
            {0 1 0 0}
            {0 1 1 0}
            {0 0 0 0}
        }
        L2 {
            {0 0 1 0}
            {1 1 1 0}
            {0 0 0 0}
            {0 0 0 0}
        }
        L3 {
            {1 1 0 0}
            {0 1 0 0}
            {0 1 0 0}
            {0 0 0 0}
        }
        L4 {
            {0 0 0 0}
            {1 1 1 0}
            {1 0 0 0}
            {0 0 0 0}
        }
        r1 {
            {0 1 1 0}
            {0 1 0 0}
            {0 1 0 0}
            {0 0 0 0}
        }
        r2 {
            {1 0 0 0}
            {1 1 1 0}
            {0 0 0 0}
            {0 0 0 0}
        }
        r3 {
            {0 1 0 0}
            {0 1 0 0}
            {1 1 0 0}
            {0 0 0 0}
        }
        r4 {
            {0 0 0 0}
            {1 1 1 0}
            {0 0 1 0}
            {0 0 0 0}
        }
        T1  {
            {0 0 0 0}
            {1 1 1 0}
            {0 1 0 0}
            {0 0 0 0}
        }
        T2  {
            {0 1 0 0}
            {0 1 1 0}
            {0 1 0 0}
            {0 0 0 0}
        }
        T3  {
            {0 1 0 0}
            {1 1 1 0}
            {0 0 0 0}
            {0 0 0 0}
        }
        T4  {
            {0 1 0 0}
            {1 1 0 0}
            {0 1 0 0}
            {0 0 0 0}
        }
    }

    set Rotation {
        [] []
        I1 I2
        I2 I1
        Z1 Z2
        Z2 Z1
        L1 L2
        L2 L1
        S1 S2
        S2 S1
        L1 L2
        L2 L3
        L3 L4
        L4 L1
        r1 r2
        r2 r3
        r3 r4
        r4 r1
        T1 T2
        T2 T3
        T3 T4
        T4 T1
    }

    proc EmptyTable {} {
        variable Rows
        variable Columns
        lrepeat $Rows [lrepeat $Columns 0]
    }

    proc RandomBrick {} {
        variable Bricks
        set keys [dict keys $Bricks]
        lindex $keys [expr {int(rand()*[llength $keys])}]
    }

    proc BrickMatrix {name} {
        variable Bricks
        dict get $Bricks $name
    }

    proc InitialY {name} {
        set matrix [BrickMatrix $name]
        set Y -3
        for {set row 3} {$row >= 0} {incr row -1} {
            for {set col 0} {$col < 4} {incr col} {
                if {[lindex $matrix $row $col] != 0} {
                    return $Y
                }
            }
            incr Y
        }
        return $Y
    }

    proc NewBrick {} {
        variable BrickColorIndex
        variable BrickName
        variable BrickX
        variable BrickY
        variable Columns
        set BrickName [RandomBrick]
        set BrickColorIndex [RandomColorIndex]
        set BrickY [InitialY $BrickName]
        set BrickX [expr {($Columns - 4) / 2}]
    }

    proc RotatedBrick {} {
        variable Rotation
        variable BrickName
        dict get $Rotation $BrickName
    }

    proc PutBrick {TableVar} {
        upvar 1 $TableVar Table
        variable BrickX
        variable BrickY
        variable BrickName
        variable BrickColorIndex
        variable Rows
        variable Columns
        variable Colors
        set brickMatrix [BrickMatrix $BrickName]
        set result 0
        for {set i 0} {$i < 4} {incr i} {
            set row [expr {$i + $BrickY}]
            for {set j 0} {$j < 4} {incr j} {
                if {[lindex $brickMatrix $i $j]} {
                    if { $row >= $Rows } {
                        incr result
                        continue
                    }
                    set col [expr {$j + $BrickX}]
                    if { $col < 0  || $col >= $Columns} {
                        incr result
                        continue
                    }
                    if {$row < 0} {
                        continue
                    }
                    if {[lindex $Table $row $col] != 0} {
                        incr result
                    }
                    lset Table $row $col $BrickColorIndex
                }
            }
        }
        set result
    }

    proc ShowTable {Table} {
        variable Canvas
        if {![info exists Canvas]} {
            return
        }
        variable Rows
        variable Columns
        variable Tiles
        variable Colors

        for {set row 0} {$row < $Rows} {incr row} {
            for {set col 0} {$col < $Columns} {incr col} {
                set color [lindex $Colors [lindex $Table $row $col]]
                set tile [lindex $Tiles $row $col]
                $Canvas itemconfigure $tile -fill $color
            }
        }
    }

    proc MoveDown {} {
        variable Freeze
        if {![info exist Freeze] || $Freeze != 0} {
            return
        }
        variable DisplayTable
        variable StaticTable
        variable BrickY
        set NewTable $StaticTable
        incr BrickY
        set conflict [PutBrick NewTable]
        if {$conflict > 0} {
            set StaticTable $DisplayTable
            CheckCompleteRows StaticTable
            if {$BrickY >= 0} {
                NewBrick
                set NewTable $StaticTable
                PutBrick NewTable
            } else {
                GameOver
                return
            }
        }
        set DisplayTable $NewTable
        ShowTable $DisplayTable
    }

    proc MoveLeft {} {
        variable DisplayTable
        variable StaticTable
        variable BrickX
        set NewTable $StaticTable
        incr BrickX -1
        set conflict [PutBrick NewTable]
        if {$conflict > 0} {
            incr BrickX
            return
        }
        set DisplayTable $NewTable
        ShowTable $DisplayTable
    }

    proc MoveRight {} {
        variable DisplayTable
        variable StaticTable
        variable BrickX
        set NewTable $StaticTable
        incr BrickX
        set conflict [PutBrick NewTable]
        if {$conflict > 0} {
            incr BrickX -1
            return
        }
        set DisplayTable $NewTable
        ShowTable $DisplayTable
    }

    proc Rotate {} {
        variable StaticTable
        variable DisplayTable
        variable BrickX
        variable BrickName
        set NewTable $StaticTable
        set PrevBrick $BrickName
        set BrickName [RotatedBrick]

        set conflict [PutBrick NewTable]
        if {$conflict > 0} {
            incr BrickX -1
            set NewTable $StaticTable
            set conflict [PutBrick NewTable]
            if {$conflict} {
                incr BrickX 2
                set NewTable $StaticTable
                set conflict [PutBrick NewTable]
                if {$conflict} {
                    incr BrickX -1
                    set BrickName $PrevBrick
                    return
                }
            }
        }
        set DisplayTable $NewTable
        ShowTable $DisplayTable
    }

    proc CheckCompleteRows {TableVar} {
        variable Rows
        variable Columns
        variable Top
        upvar 1 $TableVar Table
        set found 0
        set newTable [list]
        set tempTable [list]
        set emptyRow [lrepeat $Columns 0]
        set emptyRows [list]
        foreach row $Table {
            if {[lsearch -integer $row 0] >= 0} {
                lappend newTable $row
                lappend tempTable $row
            } else {
                lappend emptyRows $emptyRow
                lappend tempTable $emptyRow
            }
        }
        if {[llength $emptyRows]} {
            DisableKeys
            try {
                for {set i 0} {$i < 10} {incr i} {
                    ShowTable $tempTable
                    Wait 10
                    ShowTable $Table
                    Wait 10
                }
            } finally {
                EnableKeys
                set Table [concat $emptyRows $newTable]
                variable Speed
            }
            if {[info exists Speed] && $Speed > 40} {
                set Speed [expr {int( ( $Speed - 40 ) * 0.995 ) + 40}]
            }
        }
    }

    variable LightShow 0

    proc LightShow {} {
        variable LightShow
        variable Colors
        variable Canvas
        variable Tiles
        if {[info exists LightShow]} {
            if {$LightShow > 0} {
                foreach tileRow $Tiles {
                    foreach tile $tileRow {
                        $Canvas itemconfigure $tile -fill [lindex $Colors [RandomColorIndex]]
                    }
                }
                after $LightShow [namespace current]::LightShow
            } else {
                NewGame
            }
        }
    }

    proc NewGame {} {
        variable StaticTable
        variable DisplayTable
        variable Speed

        set Speed 1000
        set StaticTable [EmptyTable]

        NewBrick

        set DisplayTable $StaticTable
        PutBrick DisplayTable
        ShowTable $DisplayTable

        EnableKeys

        after $Speed [namespace current]::CyclicMoveDown
        return
    }

    proc CyclicMoveDown {} {
        variable Freeze
        variable Speed
        variable LightShow
        if {[info exists LightShow] && $LightShow == 0 && [info exists Speed]} {
            MoveDown
            after $Speed [namespace current]::CyclicMoveDown
        }
    }

    proc GameOver {} {
        DisableKeys
        variable LightShow

        set LightShow 10
        LightShow

        after 2000 [list namespace inscope [namespace current] {
            set LightShow 0
        }]
    }

} ; # namespace eval mdbg::shell::bricks

namespace eval mdbg::shell {
    proc bricks! {} {
        bricks::Start
    }
} ; # namespace eval mdbg::shell