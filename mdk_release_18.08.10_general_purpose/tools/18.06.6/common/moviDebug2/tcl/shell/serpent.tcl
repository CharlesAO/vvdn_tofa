# ***************************************************************************
# Copyright (C) 2017 Movidius Ltd. All rights reserved
# ***************************************************************************
namespace eval mdbg::shell::serpent {

    proc Start {} {
        package require Tk

        variable Top
        variable Canvas

        catch {wm withdraw .}

        set Top .serpent!

        toplevel $Top
        set Canvas [tk::canvas $Top.canvas -background black]

        wm title $Top "Serpent!"
        wm resizable $Top 0 0

        pack $Canvas

        variable Dark
        set Dark #101010

        variable Colors [list $Dark red orange yellow lime green cyan blue magenta]

        variable Rows    20
        variable Columns 40
        variable ScaleX  16
        variable ScaleY  16

        $Canvas configure -width [expr {$Columns * $ScaleX}] -height [expr {$Rows * $ScaleY}]

        bind $Canvas <Destroy> [list namespace inscope [namespace current] {
            CancelAdvance
            unset -nocomplain Canvas
            unset -nocomplain Speed
            unset -nocomplain FoodPosition
        }]

        bind $Top <Key-Escape> [list destroy $Top]

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
                    [expr {$ScaleX * ( $col + 1 ) - 0}] [expr {$ScaleY * ( $row + 1 ) - 0}]
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

    proc MixColors {from to blend} {
        if {$from == $to} {
            return $from
        }
        if {$blend >= 1.0} {
            return $to
        } elseif {$blend <= 0} {
            return $from
        }
        variable Top
        lassign [winfo rgb $Top $from ] rf gf bf
        lassign [winfo rgb $Top $to] rt gt bt
        set alpha [expr {255.0 * (1.0 - $blend) / 65535.0}]
        set beta [expr {255.0 * ($blend) / 65535.0}]

        set r [expr {min(255,int( $rf * $alpha + $rt * $beta))}]
        set g [expr {min(255,int( $gf * $alpha + $gt * $beta))}]
        set b [expr {min(255,int( $bf * $alpha + $bt * $beta))}]

        format #%02x%02x%02x $r $g $b
    }

    proc EnableKeys {} {
        variable Top
        variable Freeze

        set Freeze 0

        bind $Top <Key-Left>  "[namespace current]::Move Left"
        bind $Top <Key-Right> "[namespace current]::Move Right"
        bind $Top <Key-Down>  "[namespace current]::Move Down"
        bind $Top <Key-Up>    "[namespace current]::Move Up"
    }

    proc DisableKeys {} {
        variable Top
        variable Freeze
        set Freeze 1
        bind $Top <Key-Left> {}
        bind $Top <Key-Right> {}
        bind $Top <Key-Down> {}
        bind $Top <Key-Up>   {}
    }

    proc Wait {ms} {
        after $ms incr dummy
        vwait dummy
    }

    variable Pieces {}

    proc EmptyTable {} {
        variable Rows
        variable Columns
        lrepeat $Rows [lrepeat $Columns ""]
    }

    proc NewFood {} {
        variable FoodPosition
        variable FoodColor
        variable LastFoodColorIndex
        variable FoodStartMs
        variable Colors

        variable Table
        variable Rows
        variable Columns
        set pl [list]
        for {set i 0} {$i < $Rows} {incr i} {
            for {set j 0} {$j < $Columns} {incr j} {
                if {[lindex $Table $i $j] == ""} {
                    lappend pl [list $i $j]
                }
            }
        }
        if {[set len [llength $pl]]} {
            set FoodPosition [lindex $pl [expr {int(rand() * $len)}]]
            if {[llength $Colors] <= 2} {
                set color [lindex $Colors 1]
            } else {
                set color $FoodColor
                while {$color == $FoodColor} {
                    set color [lindex $Colors [RandomColorIndex]]
                }
            }
            set FoodColor $color
            set FoodStartMs [clock milliseconds]
            DrawFood $FoodStartMs
        } else {
            unset -nocomplain FoodPosition
            unset -nocomplain FoodColor
        }
    }

    proc DrawFood {clock_ms} {
        variable FoodPosition
        variable FoodColor
        variable FoodStartMs
        variable Table
        variable Tiles
        variable Canvas
        variable Dark

        set blend [expr { 0.75 * ((cos( ($clock_ms - $FoodStartMs) * 3.141592 / 750) + 1) / 2)}]
        set color [MixColors $FoodColor $Dark $blend]

        lassign $FoodPosition row col

        lset Table $row $col $color
        $Canvas itemconfigure [lindex $Tiles $row $col] -fill $color
    }

    proc ShowTable {} {
        variable Table
        variable Rows
        variable Columns
        variable Tiles
        variable Dark
        variable Canvas

        for {set row 0} {$row < $Rows} {incr row} {
            for {set col 0} {$col < $Columns} {incr col} {
                set color [lindex $Table $row $col]
                set tile [lindex $Tiles $row $col]
                if {$color == ""} {
                    set color $Dark
                }
                $Canvas itemconfigure $tile -fill $color
            }
        }
    }

    proc NextPosition {position direction} {
        lassign $position row col
        switch $direction {
            Up {
                incr row -1
            }
            Down {
                incr row
            }
            Left {
                incr col -1
            }
            Right {
                incr col 1
            }
            default {
                error "invalid direction $direction"
            }
        }
        variable Columns
        variable Rows
        #list $row $col
        list \
            [expr {( $row + $Rows ) % $Rows}] \
            [expr {( $col + $Columns ) % $Columns}]
    }

    proc Move {direction} {
        variable HeadDirection
        variable PiecePositions

        lassign [NextPosition [lindex $PiecePositions end] $direction] row col
        if {[llength $PiecePositions] > 1} {
            lassign [lindex $PiecePositions end-1] norow nocol
            if {$norow == $row && $nocol == $col} {
                lassign [lindex $PiecePositions 0] headrow headcol
                lassign [lindex $PiecePositions 1] prevrow prevcol
                if {$prevrow < $headrow} {
                    set HeadDirection Down
                } elseif {$prevrow > $headrow} {
                    set HeadDirection Up
                } elseif {$prevcol < $headcol} {
                    set HeadDirection Right
                } elseif {$prevcol > $headcol} {
                    set HeadDirection Left
                } else {
                    return
                }
                set PiecePositions [lreverse $PiecePositions]
                CancelAdvance

                variable PieceColors
                set origColors $PieceColors
                DisableKeys
                try {
                    set colors1 [lrange $PieceColors 0 [expr {[llength $PiecePositions]-1}]]
                    set colors2 [lreverse $colors1]
                    for {set i 0} {$i < 10} {incr i} {
                        set tempColors [list]
                        set blend [expr {$i / 10.0}]
                        foreach c1 $colors1 c2 $colors2 {
                            lappend tempColors [MixColors $c2 $c1 $blend]
                        }
                        set PieceColors $tempColors
                        DrawPieces
                        ShowTable
                        Wait 10
                    }
                } finally {
                    set PieceColors $origColors
                    EnableKeys
                }
                ScheduleAdvance
                return
            }
        }
        set HeadDirection $direction
        CancelAdvance
        Advance
        ScheduleAdvance
    }

    proc Advance {} {
        variable PiecePositions
        variable PieceColors
        variable HeadDirection
        variable FoodPosition
        variable Rows
        variable Columns
        variable Table
        variable Canvas

        lassign [NextPosition [lindex $PiecePositions end] $HeadDirection] row col

        if {$row < 0 || $row >= $Rows || $col < 0 || $col >= $Columns} {
            GameOver
            return
        }

        if {[info exists FoodPosition]} {
            lassign $FoodPosition frow fcol
            if {$frow == $row && $fcol == $col} {
                variable FoodColor
                EatColor $FoodColor 4
                NewFood
                lset Table $row $col ""
            }
        }

        if {[lindex $Table $row $col] != ""} {
            GameOver
            return
        }

        lappend PiecePositions [list $row $col]

        if {[llength $PiecePositions] > [llength $PieceColors]} {
            set PiecePositions [lassign $PiecePositions tail]
            lassign $tail trow tcol
            lset Table $trow $tcol ""
        }

        DrawPieces
        ShowTable
    }

    proc EatColor {color length} {
        variable PieceColors
        set head [lindex $PieceColors end]
        for {set i 1} {$i <= $length} {incr i} {
            lappend PieceColors [MixColors $head $color [expr {$i * 1.0 / $length}]]
        }
    }

    proc NewGame {} {
        variable Speed 400

        variable Rows
        variable Columns
        variable Colors
        variable Dark

        set row [expr {$Rows / 2}]
        set col [expr {$Columns / 2}]

        variable PiecePositions [list [list $row $col]]
        set headColor [lindex $Colors [RandomColorIndex]]
        set tailColor [MixColors $Dark $headColor .25]
        variable HeadDirection Right

        variable PieceColors [list $tailColor]
        EatColor $headColor 4
        variable FoodColor $headColor

        variable Table [EmptyTable]

        DrawPieces

        NewFood

        ShowTable

        EnableKeys

        ScheduleAdvance

        CyclicDrawFood

        return
    }

    proc DrawPieces {} {
        variable PiecePositions
        variable PieceColors
        variable Table

        set colors [lrange $PieceColors 0 [expr {[llength $PiecePositions] - 1}]]

        foreach pos $PiecePositions color $colors {
            lassign $pos row col
            lset Table $row $col $color
        }
    }

    proc FinalAnimation {} {
        variable Rows
        variable FinalAnimationRow $Rows
        AnimateRowFill
    }

    proc AnimateRowFill {} {
        variable Canvas
        if {![info exists Canvas]} {
            return
        }
        variable FinalAnimationRow
        incr FinalAnimationRow -1
        if {$FinalAnimationRow < 0} {
            unset FinalAnimationRow
            NewGame
            return
        }
        variable Columns
        variable Tiles
        variable Colors
        foreach tile [lindex $Tiles $FinalAnimationRow] {
            $Canvas itemconfigure $tile -fill [lindex $Colors [RandomColorIndex]]
        }
        after 40 [namespace current]::AnimateRowFill
    }

    proc CancelAdvance {} {
        variable CyclicAdvanceId
        if {[info exists CyclicAdvanceId]} {
            after cancel $CyclicAdvanceId
            unset CyclicAdvanceId
        }
    }

    proc ScheduleAdvance {} {
        variable CyclicAdvanceId
        variable Speed
        if {[info exists Speed]} {
            set speed $Speed
            variable PiecePositions
            variable PieceColors
            if {[info exists PiecePositions] && [info exists PieceColors] && [llength $PieceColors] > [llength $PiecePositions]} {
                set speed [expr {$speed / 2}]
            }
            set CyclicAdvanceId [after $speed [namespace current]::CyclicAdvance]
        }
    }

    proc CyclicAdvance {} {
        variable CyclicAdvanceId
        if {![info exists CyclicAdvanceId]} {
            return
        }
        Advance
        ScheduleAdvance
    }

    proc CyclicDrawFood {} {
        variable FoodPosition
        if {[info exists FoodPosition]} {
            DrawFood [clock milliseconds]
            after 40 [namespace current]::CyclicDrawFood
        }
    }

    proc GameOver {} {

        CancelAdvance
        DisableKeys

        variable HeadDirection
        unset -nocomplain HeadDirection

        variable FoodPosition
        unset -nocomplain FoodPosition

        variable Speed
        unset -nocomplain Speed
        FinalAnimation
    }

} ; # namespace eval mdbg::shell::serpent

namespace eval mdbg::shell {
    proc serpent! {} {
        serpent::Start
    }
} ; # namespace eval mdbg::shell