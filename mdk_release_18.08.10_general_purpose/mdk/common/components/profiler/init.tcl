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

    set elf [file normalize "[pwd]/$elf"]
    set sessionDir $::SESSION_DIR
    if { [regexp {^[^\/].*$} $sessionDir ] } {
        set sessionDir "[pwd]/$sessionDir"
    }

    file mkdir $sessionDir
    file copy $elf $sessionDir
    set elf [file join $sessionDir [file tail $elf]]

	if { ! [info exists SAMPLE_PERIOD_MICRO] } {
		set SAMPLE_PERIOD_MICRO 0
	}

	set profArgs [string map {; ;\n} $profArgs]
	append profArgs "platform = \"[getPlatform]\";\n"
	append profArgs "elfPath = \"$elf\";\n"

    if { [regexp {build (\d+)\.} [exec {*}${moviProf} --version] v versionNumber] } {
    if { $versionNumber < 20180315 } {
        # deprecated. remove it when nobody will use it
        puts "WARNING: you are using a deprecated moviProf, please upgrade"
        set moviProf "$moviProf --lttng=lttng_[exec date +%Y.%m.%d-%H.%M.%S]"
        old_decodeBuffers ${elf} "${moviProf}" ${profArgs} ${SAMPLE_PERIOD_MICRO}
    } {
        # this will be deprecated by nextgen
        decodeBuffers ${elf} "${moviProf}" ${profArgs} ${SAMPLE_PERIOD_MICRO}
    }
    samplePrint ${elf}
    } {
        puts "ERROR: unknown moviProf. Check instalation"
    }
    }]} {
        puts stderr $::errorInfo
    }
}

# When hit, download buffer and continue execution. in parralel or not
# FIXME: figure out a solution to inhibit runw termination when this particular
# breakpoint is hit.
#bp add collectProfileData { code to run when this is hit }
#collect function is present for each core...

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

proc decodeBuffers { elf moviProf profArgs samplePeriod } {
    # search for all master resords in elf
    set PbSyms [sym list -q *__profileBufferMasterRecord__*]
    set sampleFiles ""
    array set minBufferSize [list LOS 0 LRT 0 SHV 0]
    foreach sym $PbSyms {
        set masterRecord [dict get $sym Name]
        set masterRecordAddr [expr [dict get $sym Address] & ~0x40000000 ]
        puts "\n===========================\nFound master record ${masterRecord} at address $masterRecordAddr"
        set magic [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->magic -value]
        if { $magic != 0x666f7270 } {
            puts "ERROR: Invalid magic number on master record: $magic"
            continue
        }
        set version [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->version -value]
        set coreId [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->coreId -value]
        set coreName SHV
        if { $coreId == 1 } {
            set coreName LOS
        } elseif { $coreId == 2 } {
            set coreName LRT
        }
        set startTime [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->startTime -value]
        set coreSysClock [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->clockSpeed -value]
        #puts "startTime = $startTime"
        if { $coreId == 0xFF } {
            puts "INFO: No data available in core ${coreId}. Skipping..."
            continue
        }
        if { $coreSysClock != 0 && $coreSysClock != -1 } {
            puts "INFO: Detected sys clock speed kHz = $coreSysClock"
            set ::sysClock $coreSysClock
        }
        set myriadChipVersion [mget *((ProfileBufferMasterRecord_t*)${masterRecordAddr})->myriadChipVersion -value -binary -size 6 -scan a* ]
        set fProf [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->fProf -value]
        set tProf [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->tProf -value]
        set sProf [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->sProf -value]


        foreach { pType pprof } "trace $tProf function $fProf sample $sProf" {
            if { !$pprof } {
                puts "INFO: The $pType profiling type is not used by the core $coreId"
                continue
            }
            set head [mget ((ProfileBufferEntry_t*)${pprof})->head -value -type pointer]
            set bufferEnd [mget ((ProfileBufferEntry_t*)${pprof})->end -value -type pointer]
            set startBuffer [mget ((ProfileBufferEntry_t*)${pprof})->buffer -value -type pointer]

            set bufferSize [expr $bufferEnd - $startBuffer]

            puts "INFO: ${pType} profile buffer v.[format %d $version] CoreID: $coreId ($myriadChipVersion)\n\
                \[${startBuffer} .. *${head} .. [format 0x%X [expr $startBuffer + $bufferSize]]\] (size: $bufferSize)"
            if { ${head} < ${startBuffer} || ${startBuffer} + ${bufferSize} <= ${head} } {
                puts "ERROR: Buffer appears to be invalid. Skipping..."
                continue
            }

            close [file tempfile fileName "${pType}${masterRecord}${coreId}.bin"]
            # if we have data at head on a length of 4, then we have two chunks of data
            if { [lsearch -not -exact -integer [mget $head -value -count 4] 0] == -1 } {
                #is zero, then only one
                set usedBuffer [expr {${head} - $startBuffer}]
                set bufferSize [expr {$bufferEnd-$startBuffer}]
                eval% savefile $fileName $startBuffer $usedBuffer
                puts "INFO: Used buffer: ${usedBuffer}/${bufferSize} bytes ([expr {($usedBuffer * 100.0)/$bufferSize}]%)"
                if { $usedBuffer > $minBufferSize($coreName) && $minBufferSize($coreName) != -1 } {
                    set minBufferSize($coreName) $usedBuffer
                }
            } else {
                puts "two chunks because we have data at write pointer: [mget $head -value -count 4]"
                # two chunks
                eval% savefile $fileName $head [expr {$bufferEnd - ${head}}]
                eval% savefile -append $fileName $startBuffer [expr {${head} - $startBuffer}]
                set bufferSize [expr {$bufferEnd-$startBuffer}]
                puts "Used buffer: $bufferSize bytes from $bufferSize (100.0%)"
                set minBufferSize($coreName) -1
            }

    set sl [list startTime ${startTime}]
    lappend sl fileNames [::json::write array \"$fileName\"]
    lappend sl coreId $coreId
    lappend sl type \"$pType\"
    lappend sl elf \"$elf\"

    #lappend jsettings {*}[list [::json::write object {*}$sl]]
    lappend jsettings [::json::write object {*}$sl]

#           if { $pType == "sample" } {
#                # the core in this case is not important
#                set sampleFiles [concat $sampleFiles $fileName]
#            } {
#                puts "Decoding..."
#                set mprofArgs "--streamVersion=${version} -t${pType} -c${coreId} --verbose --startTime=${startTime} --elf=$elf --clocks=$::sysClock"
#                if { [catch { exec {*}${moviProf} {*}${mprofArgs} ${fileName} --arg=${profArgs} } msg] } {
#                    puts "${moviProf} ${mprofArgs} $fileName --arg=${profArgs}"
#                    puts "${msg}"
#                    continue
#                }
#                puts "${msg}"
#            }
        }
	}
#    puts "Sample Decoding ..."
#    if { $sampleFiles != "" && [catch { exec {*}${moviProf} --streamVersion=${version} --arg=${profArgs} -tsample --intervalSample=${samplePeriod} -c1 --verbose --clocks=$::sysClock --elf=$elf {*}$sampleFiles } msg] } {
#        puts $msg
#    }

    puts  [::json::write array {*}$jsettings]
    set jsettings [::json::write object verbose true arg [::json::write string $profArgs] initialClocks $::sysClock streamVersion $version intervalSample $samplePeriod bufferList [::json::write array {*}$jsettings]]
    set fileId [file tempfile jsonFile .json]
    puts $fileId $jsettings
    close $fileId
    if { [catch { eval% exec {*}${moviProf} ${jsonFile} } msg] } {
        puts "${moviProf} ${jsonFile}"
    }
    puts "${msg}"



	# TODO: enhance profiler with cache hit/miss info
	#puts "get register SVU_PCCn - 04_02_Shave_DCU.odt"
	#puts "get reg L2C_MISS_CNT, L2C_HIT_CNT, L2C_LEON_HIT_CNTR, L2C_LEON_ACCESS_CNTR"
    #
    puts "Recommended profile buffer sizes:\n\tCORE\tmin bytes"
    foreach coreName "LOS LRT SHV" {
        if { $minBufferSize($coreName) == 0 } {
            puts "\t$coreName\t<unused>"
        } elseif { $minBufferSize($coreName) != -1 } {
            puts "\t$coreName\t$minBufferSize($coreName)"
        } else {
            puts "\t$coreName\tbuffer completelly used! You might consider increasing the size..."
        }
    }

	puts "Done."
}

# TODO: refactor this
proc old_decodeBuffers { elf moviProf profArgs samplePeriod } {
    # search for all master resords in elf
    set PbSyms [sym list -q *__profileBufferMasterRecord__*]
    set sampleFiles ""
    array set minBufferSize [list LOS 0 LRT 0 SHV 0]
    foreach sym $PbSyms {
        set masterRecord [dict get $sym Name]
        set masterRecordAddr [dict get $sym Address]
        puts "\n===========================\nFound master record ${masterRecord} at address $masterRecordAddr"
        set magic [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->magic -value]
        if { $magic != 0x666f7270 } {
            puts "ERROR: Invalid magic number on master record: $magic"
            continue
        }
        set version [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->version -value]
        set coreId [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->coreId -value]
        set coreName SHV
        if { $coreId == 1 } {
            set coreName LOS
        } elseif { $coreId == 2 } {
            set coreName LRT
        }
        set startTime [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->startTime -value]
        set coreSysClock [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->clockSpeed -value]
        #puts "startTime = $startTime"
        if { $coreId == 0xFF } {
            puts "INFO: No data available in core ${coreId}. Skipping..."
            continue
        }
        if { $coreSysClock != 0 && $coreSysClock != -1 } {
            puts "INFO: Detected sys clock speed kHz = $coreSysClock"
            set ::sysClock $coreSysClock
        }
        set myriadChipVersion [mgets (*((ProfileBufferMasterRecord_t*)${masterRecordAddr})).myriadChipVersion]
        set fProf [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->fProf -value]
        set tProf [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->tProf -value]
        set sProf [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->sProf -value]
        set pProf [mget ((ProfileBufferMasterRecord_t*)${masterRecordAddr})->pProf -value]
        foreach { pType pprof } "trace $tProf function $fProf sample $sProf pgo $pProf" {
            if { !$pprof } {
                puts "INFO: The $pType profiling type is not used by the core $coreId"
                continue
            }
            set head [mget ((ProfileBufferEntry_t*)${pprof})->head -value -type pointer]
            set bufferEnd [mget ((ProfileBufferEntry_t*)${pprof})->end -value -type pointer]
            set startBuffer [mget ((ProfileBufferEntry_t*)${pprof})->buffer -value -type pointer]

            set bufferSize [expr $bufferEnd - $startBuffer]

            puts "INFO: ${pType} profile buffer v.[format %d $version] CoreID: $coreId ($myriadChipVersion)\n\
                \[${startBuffer} .. *${head} .. [format 0x%X [expr $startBuffer + $bufferSize]]\] (size: $bufferSize)"
            if { ${head} < ${startBuffer} || ${startBuffer} + ${bufferSize} <= ${head} } {
                puts "ERROR: Buffer appears to be invalid. Skipping..."
                continue
            }

            close [file tempfile fileName "${pType}${masterRecord}${coreId}.bin"]
            # if we have data at head on a length of 4, then we have two chunks of data
            if { [lsearch -not -exact -integer [mget $head -value -count 4] 0] == -1 } {
                #is zero, then only one
                set usedBuffer [expr {${head} - $startBuffer}]
                set bufferSize [expr {$bufferEnd-$startBuffer}]
                eval% savefile $fileName $startBuffer $usedBuffer
                puts "INFO: Used buffer: ${usedBuffer}/${bufferSize} bytes ([expr {($usedBuffer * 100.0)/$bufferSize}]%)"
                if { $usedBuffer > $minBufferSize($coreName) && $minBufferSize($coreName) != -1 } {
                    set minBufferSize($coreName) $usedBuffer
                }
            } else {
                puts "two chunks because we have data at write pointer: [mget $head -value -count 4]"
                # two chunks
                eval% savefile $fileName $head [expr {$bufferEnd - ${head}}]
                eval% savefile -append $fileName $startBuffer [expr {${head} - $startBuffer}]
                set bufferSize [expr {$bufferEnd-$startBuffer}]
                puts "Used buffer: $bufferSize bytes from $bufferSize (100.0%)"
                set minBufferSize($coreName) -1
            }
           if { $pType == "sample" } {
                # the core in this case is not important
                set sampleFiles [concat $sampleFiles $fileName]
            } {
                puts "Decoding..."
                set mprofArgs "--streamVersion=${version} -t${pType} -c${coreId} --verbose --startTime=${startTime} --elf=$elf --clocks=$::sysClock"
                if { [catch {exec {*}${moviProf} {*}${mprofArgs} ${fileName} --arg=${profArgs} } msg] } {
                    puts "${moviProf} ${mprofArgs} $fileName --arg=${profArgs}"
                }
                puts "${msg}"
                if { $pType == "pgo" } {
                    set profData [string map {moviProf llvm-profdata} [string range $moviProf 0 [expr [string first " " $moviProf] - 1]]]
                    puts "Merging llvm-data file in $sessionDir ..."
                    exec $profData merge default.profraw -output=$sessionDir/mvprof.profraw
                    file delete default.profraw
                }
            }
        }
	}
    puts "Sample Decoding ..."
    if { $sampleFiles != "" && [catch { exec {*}${moviProf} --streamVersion=${version} --arg=${profArgs} -tsample --intervalSample=${samplePeriod} -c1 --verbose --clocks=$::sysClock --elf=$elf {*}$sampleFiles } msg] } {
        puts $msg
    }

	# TODO: enhance profiler with cache hit/miss info
	#puts "get register SVU_PCCn - 04_02_Shave_DCU.odt"
	#puts "get reg L2C_MISS_CNT, L2C_HIT_CNT, L2C_LEON_HIT_CNTR, L2C_LEON_ACCESS_CNTR"
    #
    puts "Recommended profile buffer sizes:\n\tCORE\tmin bytes"
    foreach coreName "LOS LRT SHV" {
        if { $minBufferSize($coreName) == 0 } {
            puts "\t$coreName\t<unused>"
        } elseif { $minBufferSize($coreName) != -1 } {
            puts "\t$coreName\t$minBufferSize($coreName)"
        } else {
            puts "\t$coreName\tbuffer completelly used! You might consider increasing the size..."
        }
    }

	puts "Done."
}

# stop the application by force after timeout ms
if { ${timeout} != -1 } {
	set bomb [after ${timeout} {
        puts "Stopping Myriad Application because timeout (${timeout} ms) expired!"
        mdbg::halt -all -async
    }]
}

