namespace eval mdbg {
    namespace eval uart {

        proc stop {} {
        }

        proc AUTO_INIT {} {
        }

    }

    proc directBoardReset {} {
        mvproto::lock {
            if {[mvproto::getCurrentChip] != 1} {
                error "current chip must be 1 for CoreSight TAP access"
            }
            if {[mvproto::jtagIdCode] != 0x6BA00477} {
                error "Unexpected IDCode [mvproto::jtagIdCode]"
            }

            if {[mvproto::getCurrentSubsystem] != 0} {
                error "current subsystem must be 0 for APB access"
            }

            try {
               mvproto::setCurrentChip 0
               mvproto::reset
            } finally {
               mvproto::setCurrentChip 1
               mvproto::setCurrentSubsystem 0
            }
        }
    }

} ; # namespace eval mdbg
