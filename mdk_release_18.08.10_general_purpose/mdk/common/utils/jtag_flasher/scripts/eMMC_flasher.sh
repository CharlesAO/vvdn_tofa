#!/bin/bash
# This script allows the user to load a binary file in the eMMC memory at any
# location using moviDebug

# Default values
OFFSET=0
BINARY=0
PORT=30001
IP=127.0.0.1
SOC=ma2480
TAB='            '
BINARYSIZE=0
COMMANDS=0
PARTITION=0
BUSWIDTH=2

# help message
function help {
    echo "COMMAND       ${TAB} DESCRIPTION"
    echo "-b, --binary  ${TAB} Absolute path to the application's binary to"\
         "load on the eMMC memory."
    echo "-o, --offset  ${TAB} Position in the flash where the binary is"\
         "loaded. Default value: ${OFFSET}."
    echo "-p, --port    ${TAB} Port on which the MV board is connected. "\
         "Default value: ${PORT}."
    echo "-i, --ip      ${TAB} IP on which the MV board is connected. Default"\
         "value: ${IP}."
    echo "-s, --soc     ${TAB} Chip's revision. Default value: ${SOC}."
    echo "-c, --commands${TAB} Commands passed to the flasher. Default value"\
         ": ${COMMANDS}."
    echo "              ${TAB} -c=0 Load image in eMMC memory."
    echo "              ${TAB} -c=1 Erase eMMC partition."
    echo "-t, --partition${TAB}Select the eMMC partion to work on. Default"\
         "value: ${PARTITION}."
    echo "              ${TAB} -t=0 Flash BOOT0 partition."
    echo "              ${TAB} -t=1 Flash BOOT1 partition."
    echo "              ${TAB} -t=2 Flash USER partition."
    echo "-w, --width   ${TAB} Select the eMMC bus width to work on. Default"\
        "value: ${BUSWIDTH}."
    echo "              ${TAB} -w=0 eMMC bus width 1bit."
    echo "              ${TAB} -w=1 eMMC bus width 4bits."
    echo "              ${TAB} -w=2 eMMC bus width 8bits."
    echo "-h, --help    ${TAB} Help message."
    echo Example:
    echo "./eMMC_flasher.sh -b=/home/user/work/mdk/testApps/drivers/"\
    "freeRunCnt/rtems/ma2x8x/test_led/output/test_led.mvcmd -o=0x0 -p=30002"\
    "-t=0 -c=0 -w=2 -ip=192.168.238.33"
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
    -c=*|--commands=*)
    COMMANDS="${i#*=}"
    ;;
    -t=*|--partition=*)
    PARTITION="${i#*=}"
    ;;
    -w=*|--width=*)
    BUSWIDTH="${i#*=}"
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
FLASHER="utils/jtag_flasher/mmc_flasher_${SOC}.elf"
if [ ${SOC} != "ma2480" ]; then
    echo "This script supports only ma2480 version of the chip"
    exit 1;
fi

if [ ${COMMANDS} -eq 0 ]
then
    # Check if the binary specified exists
    if [ ! -e ${BINARY} ]
    then
        echo "The binary specified doesn't exist"
        echo "Help:"
        help
        exit 1
    fi

    # Calculate binary size
    BINARYSIZE=$(stat -c%s "$BINARY")
fi

echo "Load this application ${BINARY} at the location ${OFFSET} in the"\
     "eMMC memory"
# Run tcl script
make -C ../../../ -j MV_SOC_REV=$SOC srvPort=$PORT srvIP=$IP \
tcl2 CMD="set binary ${BINARY}; set binarysize ${BINARYSIZE}; \
set offset ${OFFSET}; set commands ${COMMANDS}; set partition ${PARTITION}; \
set width ${BUSWIDTH}; set flasher ${FLASHER}; source `pwd`/eMMC_flasher.tcl"
