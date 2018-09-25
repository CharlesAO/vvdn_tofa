#!/bin/bash
# This script allows the user to load a binary in the flash memory at any
# location using moviDebug

# Default values
OFFSET=0
BINARY=0
PORT=30001
IP=127.0.0.1
SOC=ma2480
TAB='            '

# help message
function help {
    echo "COMMAND      ${TAB} DESCRIPTION"
    echo "-b, --binary ${TAB} Absolute path to the application's binary to"\
         "load on the flash."
    echo "-o, --offset ${TAB} Position in the flash where the binary is"\
         "loaded. Default value: ${OFFSET}."
    echo "-p, --port   ${TAB} Port on which the MV board is connected. Default"\
         "value: ${PORT}."
    echo "-i, --ip     ${TAB} IP on which the MV board is connected. Default"\
         "value: ${IP}."
    echo "-s, --soc    ${TAB} Chip's revision. Default value: ${SOC}."
    echo "-h, --help   ${TAB} Help message."
    echo Example:
    echo "./flasher.sh -b=/home/user/work/mdk/testApps/drivers/freeRunCnt/"\
    "rtems/ma2x8x/test_led/output/test_led.mvcmd -o=0x400000 -p=30002 "\
    "-i=192.168.238.33"
}

# Parse input arguments
for i in "$@"
do
case $i in
    -b=*|--binary=*)
    BINARY="${i#*=}"
    ;;
    -o=*|--offset=*)
    OFFSET="${i#*=}"
    ;;
    -p=*|--port=*)
    PORT="${i#*=}"
    ;;
    -i=*|--ip=*)
    IP="${i#*=}"
    ;;
    -s=*|--soc=*)
    SOC="${i#*=}"
    ;;
    -h|--help)
    help
    exit 1
    ;;
    *)
        # unknown option
    ;;
esac
done

# Path from mdk/common
FLASHER="utils/jtag_flasher/flasher_${SOC}.elf"
if [ ${SOC} != "ma2480" ]; then
    echo "This script supports only ma2480 version of the chip"
    exit 1;
fi

# Check if the binary specified exist
if [ ! -e ${BINARY} ]
then
    echo "The binary specified doesn't exist"
    echo "Help:"
    help
    exit 1
fi

# Calculate binary size
BINARYSIZE=$(stat -c%s "$BINARY")

echo "Load this application ${BINARY} at the location ${OFFSET} in the"\
     "flash memory"
# Run tcl script
make -C ../../../ MV_SOC_REV=$SOC srvPort=$PORT srvIP=$IP \
tcl2 CMD="set binary ${BINARY}; set binarysize ${BINARYSIZE}; \
set offset ${OFFSET}; set flasher ${FLASHER}; source `pwd`/flasher.tcl"
