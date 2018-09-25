proc ldri {} {

    mdbg::breset
    mdbg::target LOS
    mdbg::loadfile $::elf

    set los_exists [mdbg::sym addr -nocomplain mvConsoleTxQueue]
    if { $los_exists ne "" } {
        mdbg::pipe create LOS
        mdbg::pipe configure LOS -stdout
        mdbg::pipe configure LOS -readsym mvConsoleTxQueue
    }

    set lrt_exists [mdbg::sym addr -nocomplain lrt_mvConsoleTxQueue]
    if { "$lrt_exists" ne "" } {
      mdbg::pipe create LRT
      mdbg::pipe configure LRT -stdout
      mdbg::pipe configure LRT -readsym lrt_mvConsoleTxQueue
    }
    mdbg::runw

    foreach i [mdbg::pipe list] {
        mdbg::pipe flush $i
    }

    puts "Application finished!"


}

ldri
