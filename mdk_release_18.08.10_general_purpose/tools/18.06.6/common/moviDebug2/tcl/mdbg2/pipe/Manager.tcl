# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : pipe/Manager.tcl
# Description: Debug Pipes Manager also responsible for background transfer
# Created on : Aug 10, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

package require TclOO

namespace eval mdbg::pipe {

    oo::class create PipeManager {

        variable Memory
        variable PipeMap
        variable ReadMap
        variable WriteMap
        variable ChannelMap

        variable ConsolePrefix

        variable BusyInterval
        variable IdleInterval

        variable BusyReadList
        variable IdleReadList
        variable BusyWriteList
        variable IdleWriteList

        variable BusyAfterId
        variable IdleAfterId

        constructor {memory} {
            set Memory $memory
            set PipeMap [dict create]
            set ReadMap [dict create]
            set WriteMap [dict create]
            set ChannelMap [dict create]
            set ConsolePrefix "PIPE:"

            set BusyInterval 10
            set IdleInterval 750
            set BusyReadList [list]
            set IdleReadList [list]
            set BusyWriteList [list]
            set IdleWriteList [list]
        }

        destructor {
            my reset
        }

        method consolePrefix {args} {
            set ConsolePrefix {*}$args
        }

        method setReadAddr {name address} {
            tailcall [my get $name] setReadAddr $address
        }

        method setWriteAddr {name address} {
            tailcall [my get $name] setWriteAddr $address
        }

        # 4096 is the default Tcl channel buffer size
        method toConsole {name {chan stdout} {width 4096} } {
            dict set ReadMap $name [list PipeToConsole $chan $width]
        }

        # 4096 is the default Tcl channel buffer size
        method toChan {name chan {bufsize 4096} } {
            dict set ReadMap $name [list PipeToChan $chan $bufsize]
        }

        # 4096 is the default Tcl channel buffer size
        method fromChan {name chan {bufsize 4096} } {
            dict set WriteMap $name [list ChanToPipe $chan $bufsize]
        }

        method toNone {name} {
            if {[dict exists $ReadMap $name]} {
                dict unset ReadMap $name
            }
        }

        method fromNone {name} {
            if {[dict exists $WriteMap $name]} {
                dict unset WriteMap $name
            }
        }

        method findNames { pattern } {
            dict keys $PipeMap $pattern
        }

        method getNames { } {
            dict keys $PipeMap
        }

        method create {pipeName} {
            if {[my exists $pipeName]} {
                return -code error "pipe \"$pipeName\" already exists"
            }
            my CreatePipe $pipeName
        }

        method get {pipeName} {
            if {![my exists $pipeName]} {
                return -code error "pipe \"$pipeName\" does not exist"
            }
            dict get $PipeMap $pipeName
        }

        method exists {pipeName} {
            dict exists $PipeMap $pipeName
        }

        method delete {pipeName} {
            my RemoveFromDict ReadMap $pipeName
            my RemoveFromDict WriteMap $pipeName

            my RemoveFromList BusyWriteList $pipeName
            my RemoveFromList BusyReadList $pipeName
            my RemoveFromList IdleWriteList $pipeName
            my RemoveFromList IdleReadList $pipeName

            my ClosePipeChannels $pipeName

            set pipe [my RemoveFromDict PipeMap $pipeName]
            if {$pipe != {}} {
                $pipe destroy

                if {![dict size $PipeMap]} {
                    my StopUpdate
                }
            }
        }

        method reset {} {
            my StopUpdate

            my ClearList BusyReadList
            my ClearList BusyWriteList
            my ClearList IdleReadList
            my ClearList IdleWriteList

            my ClearDict ReadMap
            my ClearDict WriteMap

            foreach item [dict value [my ClearDict ChannelMap]] {
                my CloseMapChannels $item
            }

            foreach pipe [dict values [my ClearDict PipeMap]] {
                $pipe destroy
            }

        }

        method setBusyInterval {interval} {
            my ValidateInterval $interval busy
            my CancelUpdateBusy
            set BusyInterval $interval
            my ScheduleUpdateBusy
        }

        method setIdleInterval {interval} {
            my ValidateInterval $interval idle
            my CancelUpdateIdle
            set IdleInterval $interval
            my ScheduleUpdateIdle
        }

        method getIntervals {} {
            dict create -busy $BusyInterval -idle $IdleInterval
        }

        method setPrivateChannel {pipeName category channel} {

            set setChannel [string length $channel]

            if {[dict exists $ChannelMap $pipeName $category]} {
                set prevChannel [dict get $ChannelMap $pipeName $category $channel]
                dict unset ChannelMap $pipeName $category
            }

            if {$setChannel} {
                dict set $ChannelMap $pipeName $category $channel
            }

            if {[info exists prevChannel]} {
                close $prevChannel
            }

        }

        #
        # Protected
        #

        method UpdateBusy {} {
            set newList [list]
            foreach name $BusyReadList {
                set count [my Read $name]
                if {$count > 0} {
                    lappend newList $name
                } elseif {$count == 0} {
                    lappend IdleReadList $name
                }
            }
            set BusyReadList $newList
            set newList [list]
            foreach name $BusyWriteList {
                set count [my Write $name]
                if {$count > 0} {
                    lappend newList $name
                } elseif {$count == 0} {
                    lappend IdleWriteList $name
                }
            }
            set BusyWriteList $newList
            my ScheduleUpdateBusy
        }

        export UpdateBusy

        method UpdateIdle {} {
            set newList [list]
            foreach name $IdleReadList {
                set count [my Read $name]
                if {$count > 0} {
                    lappend BusyReadList $name
                } elseif {$count == 0} {
                    lappend newList $name
                }
            }
            set IdleReadList $newList
            set newList [list]
            foreach name $IdleWriteList {
                set count [my Write $name]
                if {$count > 0} {
                    lappend BusyWriteList $name
                } elseif {$count == 0} {
                    lappend newList $name
                }
            }
            set IdleWriteList $newList
            my ScheduleUpdateIdle
        }

        export UpdateIdle

        #
        #  Private
        #
        method CreatePipe {name} {
            set pipe [::mdbg::pipe::DebugPipe new $Memory]
            if {![dict size $PipeMap]} {
                my ScheduleUpdateBusy
                my ScheduleUpdateIdle
            }

            dict set PipeMap $name $pipe
            lappend IdleReadList $name
            lappend IdleWriteList $name
            return $pipe
        }

        method PrintConsoleLine {chan name data} {
            chan puts -nonewline $chan "$ConsolePrefix$name: $data"
        }

        method PipeToConsole {chan width pipe name} {
            set data [$pipe recvLine $width]
            set count [string length $data]
            if {$count > 0} {
                if {[string index $data end] != "\n"} {
                    append data "\n"
                }
                my PrintConsoleLine $chan $name $data
            }
            return $count
        }

        method PipeToChan {chan size pipe name} {
            my NonBlocking $chan {
                set pending [chan pending output $chan]

                if {$pending < 0} {
                    return 0
                }
                set bufferSize [chan configure $chan -buffersize]

                if {0 < $bufferSize && $bufferSize < $pending} {
                    return 0
                }

                switch -exact [chan configure $chan -buffering] {
                    "line" {
                        set data [$pipe recvLine $size]
                        if {[string length $data]} {
                            chan puts -nonewline $chan $data
                            if {[string index $data end] != "\n"} {
                                chan flush $chan
                            }
                        }
                    }

                    "full" {
                        set readSize [expr { min (
                            $size,
                            $pending - $bufferSize
                        )}]
                        set data [$pipe recv $readSize]
                        if {[string length $data]} {
                            chan puts -nonewline $chan $data
                        }
                    }
                    "none" -
                    default {
                        set data [$pipe recv $size]
                        if {[string length $data]} {
                            chan puts -nonewline $chan $data
                        }
                    }

                }
                string length $data
            }
        }

        method ChanToPipe {chan size pipe name} {
            set eof [chan eof $chan]

            if {$eof && [$pipe UnwrittenLen] == 0} {
                return 0
            }

            my NonBlocking $chan {
                set data [expr {
                    $eof ? "" : [chan read $chan $size]
                }]
                set sent [$pipe send $data]
            }

            $pipe LastWriteLen
        }

        method NonBlocking {chan script} {
            set blocking [chan configure $chan -blocking]
            if {$blocking} {
                if {[catch {
                    chan configure $chan -blocking 0
                } error]} {
                    set $blocking 0
                }
            }
            try {
                set result [uplevel 1 $script]
            } finally {
                if {$blocking} {
                    chan configure $chan -blocking $blocking
                }
            }
            return $result
        }

        method Read {name} {
            if {![dict exists $PipeMap $name] || ![dict exists $ReadMap $name]} {
                return 0
            }
            set pipe [dict get $PipeMap $name]
            set readMethod [dict get $ReadMap $name]
            if {[catch {
                set count [my {*}$readMethod $pipe $name]
                if {$count < 0} {
                    set count 0
                }
            } error]} {
                puts stderr $error
                return -1
            }
            return $count
        }
        export Read
        method FlushReadAtExit {} {
            set readList [lreverse [concat $IdleReadList $BusyReadList]]
            foreach pipe $readList {
                if {[catch {
                    my FlushRead $pipe
                } error]} {
                    puts stderr "$error\n    flushing pipe \"$pipe.\""
                }
            }
        }
        export FlushReadAtExit

        method FlushRead {name} {
            if {![dict exists $PipeMap $name] || ![dict exists $ReadMap $name]} {
                return 0
            }
            set pipe [dict get $PipeMap $name]
            set readMethod [dict get $ReadMap $name]
            set max [$pipe getReadQueueSize]
            set total 0
            while {$total < $max} {
                if {[catch {
                    set count [my {*}$readMethod $pipe $name]
                    if {$count > 0} {
                        incr total $count
                    }
                } error]} {
                    puts stderr $error
                    set count 0
                }
                if {!$count} {
                    break
                }
            }
            return total
        }

        method FlushReadConsoleLines {} {
            dict for {name readMethod} $ReadMap {
                if {[lindex $readMethod 0] != "PipeToConsole"} {
                    continue
                }
                my FlushRead $name
            }
        }
        export FlushReadConsoleLines

        method Write {name} {
            if {![dict exists $PipeMap $name] || ![dict exists $WriteMap $name]} {
                return 0
            }
            set pipe [dict get $PipeMap $name]
            set writeMethod [dict get $WriteMap $name]
            if {[catch {
                set count [my {*}$writeMethod $pipe $name]
                if {$count < 0} {
                    set count 0
                }
            } error]} {
                puts stderr $error
                return -1
            }
            return $count
        }

        export Write

        method RemoveFromList {listVar pipeName} {
            upvar 1 $listVar theList
            set theList [lsearch -all -inline -exact -not $theList $pipeName]
        }

        method ClearList {listVar} {
            upvar 1 $listVar theList
            if {![info exists theList]} {
                return
            }
            set result $theList
            set theList {}
            return $result
        }

        method RemoveFromDict {dictVar pipeName} {
            upvar 1 $dictVar theDict
            if {![dict exists $theDict $pipeName]} {
                return
            }
            set result [dict get $theDict $pipeName]
            dict unset theDict $pipeName
            return $result
        }

        method ClearDict {dictVar} {
            upvar 1 $dictVar theDict
            if {![info exists theDict]} {
                return {}
            }
            set result $theDict
            set theDict {}
            return $result
        }

        method ClosePipeChannels {pipeName} {
            set map [my RemoveFromDict ChannelMap $pipeName]
            if {$map != {}} {
                my CloseMapChannels $map
            }
        }

        method CloseMapChannels {channelMapItem} {
            foreach channel [dict values $channelMapItem] {
                close $channel
            }
        }

        method ValidateInterval {interval name} {
            if {![string is integer -strict $interval] || $interval < 0} {
                error -level 0 "expected unsigned integer for $name interval"
            }
        }

        method StopUpdate {} {
            my CancelUpdateBusy
            my CancelUpdateIdle
        }

        method ScheduleUpdateBusy {} {
            set BusyAfterId [
                after $BusyInterval [self] UpdateBusy
            ]
        }

        method CancelUpdateBusy {} {
            if {[info exists BusyAfterId]} {
                after cancel $BusyAfterId
                unset BusyAfterId
            }
        }

        method ScheduleUpdateIdle {} {
            set IdleAfterId [
                after $IdleInterval [self] UpdateIdle
            ]
        }

        method CancelUpdateIdle {} {
            if {[info exists IdleAfterId]} {
                after cancel $IdleAfterId
                unset IdleAfterId
            }
        }


    } ; # oo::class PipeManager

} ; # namespace eval mdbg::pipe