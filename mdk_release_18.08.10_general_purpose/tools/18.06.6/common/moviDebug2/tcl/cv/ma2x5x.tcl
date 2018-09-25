if {[info commands mdbg::MV_SOC_REV] == ""} {
    proc mdbg::MV_SOC_REV {} { return ma2x5x }
}
source [file join [file dirname [info script]] myriad2.tcl]
mdbg::cv::myriad2::REGISTERS_FROM_HEADER "registersMyriadMa2x5x.h"
foreach script {
    gpio
    uart
} {
    source [file join [file dirname [info script]] ma2x5x ${script}.tcl]
}
mdbg::uart::AUTO_INIT