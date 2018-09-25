proc MV_SOC_REV {} { return "arm_coresight" }
# source [file dirname [file dirname [info script]]]/myriad2/registersM2.tcl
source [file dirname [info script]]/arm_coresight/reg_const.tcl
source [file dirname [info script]]/arm_coresight/reg_addr.tcl
source [file dirname [info script]]/arm_coresight/apb-ap.tcl
source [file dirname [info script]]/arm_coresight/reset.tcl

