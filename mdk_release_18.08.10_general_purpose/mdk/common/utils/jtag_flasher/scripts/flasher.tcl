mdbg::breset
mdbg::ddrinit
#Load flasher
mdbg::loadandverify $flasher

#marker:
mdbg::mset 0x80000000 0x228694d5
#size
mdbg::mset 0x80000004 $binarysize
#Set historic mode
mdbg::mset 0x80000008 0x00000008
#Sub cmd not used
mdbg::mset 0x8000000C 0
#Unused stuff (crc, etc)
mdbg::mset 0x80000010 0
mdbg::mset 0x80000014 0
mdbg::mset 0x8000001c 0
#Flash offset
mdbg::mset 0x80000018 $offset

#Load application's binary
mdbg::loadfile -addr 0x80000020 -binary $binary

mdbg::runw
exit
