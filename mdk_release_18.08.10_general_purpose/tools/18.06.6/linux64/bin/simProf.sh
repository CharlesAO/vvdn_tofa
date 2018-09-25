#!/bin/sh

if [ "$(file -ib $1)" != "application/x-executable; charset=binary" ]
then
    echo -e "Usage: $0 path/to/elf\n\nIMPORTANT: Make sure in current folder are generated
    Core_SHAVEx.out files by the moviSim -enCoreLogs"
    exit 1
fi

decode_file() {
    FILE_NAME=$1
    shaveId=$2
    OUTPUT_BUFFER=$3

    # echo "Decoding $FILE_NAME ..."
    # for ip in $(grep -Eoi 'PC=0x[0-9a-f]{8}\b' $FILE_NAME | cut -f 2 -dx)
    for ip in $(grep -Eoi 'IP = 0x[0-9a-f]{8}\b' $FILE_NAME | cut -f 2 -dx)
    do
        # print IP
        printf "\x${ip:6:2}\x${ip:4:2}\x${ip:2:2}\x${ip:0:2}"
        # print Shave ID
        printf "\x$shaveId"
    done > $OUTPUT_BUFFER
}


COMMA=
cat << EOF > /tmp/my.json
{
   "streamVersion" : 4,
   "bufferList"    : [
$(for f in Core_SHAVE*.out
do
    shaveId=${f#Core_SHAVE}
    shaveId=${shaveId%.out}
    buffer=$(mktemp)
    decode_file $f $shaveId $buffer
    echo $COMMA '{"fileNames":["'$buffer'"],'
    echo '"elf":"'$1'",'
    echo '"type":"sample"}'
    COMMA=,
done)
    ]
}
EOF

# take out the elf name
shift

$(dirname $0)/moviProf /tmp/my.json $*
