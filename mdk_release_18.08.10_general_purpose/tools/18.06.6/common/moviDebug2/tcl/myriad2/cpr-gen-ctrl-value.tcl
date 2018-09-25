namespace eval mdbg::cv::myriad2 {
    # Cached value of CPR_GEN_CTRL register
    variable CPR_GEN_CTRL_VALUE

    proc getCprGenCtrlValue {{refresh 0}} {
        variable CPR_GEN_CTRL_VALUE

        if {  ![info exists CPR_GEN_CTRL_VALUE]
            || $refresh
            || ![string is integer -strict $CPR_GEN_CTRL_VALUE]
        } {
            set CPR_GEN_CTRL_VALUE [mdbg::getReg32 CPR_GEN_CTRL_ADR]
        }
        return $CPR_GEN_CTRL_VALUE
    }

} ; # namespace eval mdbg::cv::myriad2
