proc mdbg::MV_SOC_REV {} { return vpu2x8x }

apply {{dirname scripts} {
    uplevel #0 [list source [file join $dirname myriad2.tcl]]
    mdbg::cv::myriad2::REGISTERS_FROM_HEADER "registersVPU2x8x.h"
    foreach script $scripts {
        uplevel #0 [list source [file join $dirname ma2x8x ${script}.tcl]]
    }
} :: } [file dirname [info script]] {
    gpio
    uart
}
mdbg::uart::AUTO_INIT