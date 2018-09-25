proc lddetails {} {
    mdbg::target LOS

    # Load input.general
    set paramAddr [mdbg::sym addr parameters_file]
    mdbg::loadfile -addr $paramAddr -binary  ../input/input.general

    # Load input.operations
    set operAddr [mdbg::sym addr operations_file]
    mdbg::loadfile -addr $operAddr -binary  ../input/input.operations

    #Set number of SHAVEs
    mdbg::mset numberShaves 2
}

proc ldri {} {

    # Load the elf
    mdbg::breset
    mdbg::ddrinit
    mdbg::loadfile output/test.elf

    mdbg::target LOS

    # Load details for BLISTestApp
    lddetails

    # Run the Leon
    mdbg::target LOS
    mdbg::runw
    exit
}

ldri
