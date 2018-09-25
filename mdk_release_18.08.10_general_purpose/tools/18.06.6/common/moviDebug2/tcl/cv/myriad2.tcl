namespace eval mdbg::cv::myriad2 {
    set SCRIPT_DIR [file dirname [file dirname [info script]]]

    apply [list {scripts_socx} {
        variable SCRIPT_DIR
        set MV_SOC_REV [::mdbg::MV_SOC_REV]
        set SUB_DIR    "myriad2"

        foreach {scripts socx} $scripts_socx {
            if {$socx == ".*" || [regexp ^($socx)\$ $MV_SOC_REV]} {
                foreach script $scripts {
                    if {[info exists ::mdbg::verbose] && $mdbg::verbose} {
                        puts $mdbg::Channels::verbose "loading $SUB_DIR/$script.tcl"
                    }
                    set fileName [file join ${SCRIPT_DIR} $SUB_DIR $script.tcl]
                    if {[file isfile $fileName]} {
                        uplevel #0 namespace inscope :: source $fileName
                    } else {
                        puts $mdbg::Channels::warning "warning: $fileName not found"
                    }
                }
            }
        }
    } [namespace current]] {
        {   registersM2     }     .*
        {   fragrakRegisters
            leonDefs
            board
            coreRegisters
            uartCommon
            showPC
            cprCompat
            gpioCommon
            mutexCommon     }    (ma|vpu).*
        {cpr-gen-ctrl-value
        direct-board-reset}      ma2100|(ma|vpu)2[x14][58][05x]
        cpr                      ma2100|ma2[x14][5][05x]
    }

    # rename LOAD_SCRIPT ""

    proc REGISTERS_FROM_HEADER {name} {
        variable SCRIPT_DIR
        # from tcl/cv to registers
        set registersDir [file join [file dirname [file dirname $SCRIPT_DIR]] "moviDebug2" "registers"]
        ::mdbg::REGISTERS::FROM_HEADER [file join $registersDir $name]
    }
}
