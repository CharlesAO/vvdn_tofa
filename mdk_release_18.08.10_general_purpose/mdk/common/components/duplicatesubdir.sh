#!/bin/bash

# this script is intended to be run in the directory containing the common
# components. That's $(MDK_INSTALL_DIR_ABS)/common/components

LOCAL_MK=subdir.mk

COMPONENTS_DIR=$(ls -d */ | grep -Ev 'kernelLib|vTrack.*|VcsHooks|UnitTest|Board182|shaveFifoComm')

for d in $COMPONENTS_DIR ; do
d=${d%%/}
echo $d
cp template-subdir.mk $d/$LOCAL_MK

done

# vim: set tw=0: set wrapmargin=0 :

