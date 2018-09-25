puts "INFO: Profiling buffers will be collected at exit"
namespace path [concat [namespace path] ::mdbg]
package require json::write

breset
displaystate verbose
cli::InternalErrorInfo

set sysClock 0
# collect the buffers and interpret them after the program finishes
dll::atexit {
  if {[catch {
    if { ${timeout} != -1 } {
      # minesweeper
      after cancel ${bomb}
    }
    puts "INFO: Collecting buffers ..."
    # stop everything to read consistent data
    catch { halt -a }
    # TODO: flush L1/2 cache...

    if { [info exists ::PRF_SYM_FILE] } {
      puts "Have another elf. Using $::PRF_SYM_FILE"
      set elf $::PRF_SYM_FILE
    }

    if { ! [info exists SAMPLE_PERIOD_MICRO] } {
      set SAMPLE_PERIOD_MICRO 0
    }

    set profArgs [string map {; ;\n} $profArgs]
    append profArgs "platform = \"[getPlatform]\";\n"

    file mkdir $::SESSION_DIR
    file mkdir [file join $::SESSION_DIR input]

    file copy $elf $::SESSION_DIR
    set elf [file join $::SESSION_DIR [file tail $elf]]
    append profArgs "elfPath = \"$elf\";\n"

    set bufferList [decodeBuffers ${elf}]
    set jsettings [::json::write object verbose true arg [::json::write string $profArgs] initialClocks $sysClockMhz \
        streamVersion $version intervalSample $SAMPLE_PERIOD_MICRO bufferList [::json::write array {*}$bufferList]]

    # set fileId [file tempfile jsonFile .json]
    set jsonFile [file join $::SESSION_DIR input tcl.json]
    set fileId [open $jsonFile "w"]
    puts $fileId $jsettings
    close $fileId
    if { [catch { eval% exec {*}${moviProf} ${jsonFile} -o $::SESSION_DIR } msg] } {
        puts "${moviProf} ${jsonFile}"
    }
    puts "${msg}"

    samplePrint ${elf}
    }]} {
      puts stderr $::errorInfo
    }
}

# When hit, download buffer and continue execution. in parralel or not
# FIXME: figure out a solution to inhibit runw termination when this particular
# breakpoint is hit.
#bp add collectProfileData { code to run when this is hit }
#collect function is present for each core...

set sysClockMhz 0
set version 0

# Buffer structure
#
# The entire buffer is splitted between shave cores (12 or 16) + 2 (LOS,LRT)
# | CORE_0 buffer | CORE_1 buffer | ... | CORE_n buffer |
#
# CORE_x buffer:
# | MasterRecord (32 bytes) | CHUNK_1 | CHUNK_2 | ... | CHUNK_n |
# a chunk is constant (1024 profile entries)
#
# CHUNK_x:
# | ProfileBufferEntry_t (32 bytes) | Entry | Entry | ... | Entry | 0 | 0 | ...
# Ex:
# 00000000  70 72 6f 66 06 00 00 00  20 ea a3 80 5a 00 00 00  |prof.... ...Z...|
# 00000010  28 ea a3 80 60 ae 0a 00  00 00 00 00 00 00 00 00  |(...`...........|
#           Size       |overhead    | head      | end
# 00000020  20 00 00 00 f8 ff ff ff  40 c0 39 c1 40 c0 39 c1  | .......@.9.@.9.|
# 00000030  93 19 66 0c 00 00 00 00  01 46 00 00 00 00 00 00  |..f......F......|
# 00000040  a8 27 a1 80 01 00 01 60  c8 d3 67 0c 00 00 00 00  |.'.....`..g.....|
# 00000050  60 23 a1 80 01 00 01 60  ce db 67 0c 00 00 00 00  |`#.....`..g.....|
   
proc decodeBuffers { elf } {
    global sysClockMhz
    global version
    if { ! [sym exists __profile_internal_buffer__] } {
        puts "ERROR: __profile_internal_buffer__ not found, check if loaded elf
        was instrumented"
        exit 10
    }

    # puts "TODO: now downloading the entire buffer..."
    # eval% savefile /tmp/buffer.bin __profile_internal_buffer__

    # TODO compute number of shaves 12 or 16...
    set nbShaves 16
    set nbCores [expr $nbShaves + 2]
    set coreBufferSize [expr [sym size __profile_internal_buffer__] / $nbCores]
    puts "going with coreBufferSize=$coreBufferSize"
    set masterRecord [sym addr __profile_internal_buffer__]
    set coreId 1
    set bufferList {}
    while { $coreId < $nbCores } {
        # TODO: read sample period from buffer
        set nextBuffers [download $masterRecord [expr $masterRecord + $coreBufferSize] $coreId $elf]
        lappend bufferList {*}$nextBuffers
        set masterRecord [expr $masterRecord + $coreBufferSize]
        set coreId [expr $coreId + 1]
    }

    return $bufferList
}

proc samplePrint {elf} {
    catch {
    foreach { gpfile } [glob -type f *.gprof] {
        puts "=================== ${gpfile} =================="
        # TODO: diff shall be done using gprof -u
        catch {
            # TODO: add back the --inline-file-names when even Ubuntu will support it
            eval% exec gprof -bpQ ${elf} ${gpfile}
        }

    }}
}

proc download { masterRecord masterRecordEnd coreId elf } {
    global sysClockMhz
    global version

    set magic [mget ((ProfileBufferMasterRecord_t*)$masterRecord)->magic -value]
    if { $magic != 0x666f7270 } {
        return
    }
    # TODO: test version as well
    set version [mget ((ProfileBufferMasterRecord_t*)$masterRecord)->version -value]
    set myriadChipVersion [mget ((ProfileBufferMasterRecord_t*)$masterRecord)->myriadChipVersion -value -asciiz]
    set entryCnt [mget ((ProfileBufferMasterRecord_t*)$masterRecord)->entryCnt -value]
    set toolsVersion [mget ((ProfileBufferMasterRecord_t*)$masterRecord)->toolsVersion -value -asciiz]
    set _sysClockMhz [mget ((ProfileBufferMasterRecord_t*)$masterRecord)->sysClockMhz -value]
    if { $_sysClockMhz != 0 } {
        if { $sysClockMhz != 0 && $sysClockMhz != $_sysClockMhz } {
            puts "ERROR: multiple clocks: $sysClockMhz != $_sysClockMhz"
        }
        set sysClockMhz $_sysClockMhz
    }
    puts "Buffer \[$masterRecord .. [format %x $masterRecordEnd]\] v.$version on $myriadChipVersion core: $coreId (buffers: $entryCnt) Tools: $toolsVersion Clock: $sysClockMhz"
    set profileBufferEntry 0x[format %X [expr $masterRecord + [mget sizeof(ProfileBufferMasterRecord_t) -value]]]
    set headerSize [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->headerSize -value]
    set bufferList {}
    set bufferCnt 1
    while { $headerSize != 0 } {
        # get ProfileBufferEntry_t
        if { $headerSize != [mget sizeof(ProfileBufferEntry_t) -value] } {
            puts "ERROR: Invalid ProfileBufferEntry_t size: $headerSize"
            exit 2
        }
        set overhead [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->overhead -value]
        set head [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->head -value]
        if { $head == 0 } {
            puts "ERROR: NULL head on ProfileBufferEntry_t"
            exit 3
        }
        set end [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->end -value]
        if { $end == 0 } {
            puts "ERROR: NULL end on ProfileBufferEntry_t"
            exit 4
        }
        set startTime [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->startTime -value]
        set h_coreId [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->coreId -value]
        if { $coreId != $h_coreId } {
            puts "ERROR: Wrong core id in header: $h_coreId"
            exit 5
        }
        switch [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->bufferType -value] {
            70 { set bufferType "function" }
            default {
                puts "unknown buffer type $bufferType -- "
                exit 6
            }
        }

        puts "$bufferType buffer: \[$profileBufferEntry .. $head .. $end\] overhead: $overhead ..."

        # currently the head does not rewind, in circular case the entire chunk
        # is being reused
        set fileName [file join $::SESSION_DIR input ${bufferType}${profileBufferEntry}_${coreId}_${bufferCnt}.bin]
        set bufferCnt [expr $bufferCnt + 1]
        eval% savefile $fileName [expr $profileBufferEntry + $headerSize] [expr $head - $headerSize - $profileBufferEntry]

        set sl [list startTime ${startTime}]
        lappend sl fileNames [::json::write array \"$fileName\"]
        lappend sl coreId $coreId
        lappend sl type \"$bufferType\"
        # TODO: read elf in case of dyn loaded stuff
        lappend sl elf \"$elf\"

        # lappend bufferList [list [::json::write object {*}$sl]]
        lappend bufferList [::json::write object {*}$sl]

        # get next entry
        set profileBufferEntry $end
        if { $profileBufferEntry == $masterRecordEnd } {
            set headerSize 0
            puts "expecting $entryCnt == $bufferCnt"
        } {
            set headerSize [mget ((ProfileBufferEntry_t*)$profileBufferEntry)->headerSize -value]
        }
    }

    return $bufferList
}

# stop the application by force after timeout ms
if { ${timeout} != -1 } {
	set bomb [after ${timeout} {
        puts "Stopping Myriad Application because timeout (${timeout} ms) expired!"
        mdbg::halt -all -async
    }]
}

