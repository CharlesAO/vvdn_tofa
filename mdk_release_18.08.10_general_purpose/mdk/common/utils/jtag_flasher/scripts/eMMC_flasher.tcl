mdbg::breset
mdbg::ddrinit
#Load flasher
mdbg::loadandverify $flasher

#Magic number
mdbg::mset 0x88000000 0x435259BF
#size
mdbg::mset 0x88000004 $binarysize
#eMMC offset
mdbg::mset 0x88000008 $offset
#Commands
mdbg::mset 0x8800000C $commands
#eMMC partition
mdbg::mset 0x88000010 $partition
#eMMC bus width
mdbg::mset 0x88000014 $width

#Load application's binary
if { $commands == 0 } {
    mdbg::loadfile -addr 0x88000018 -binary $binary
}
mdbg::runw
exit
