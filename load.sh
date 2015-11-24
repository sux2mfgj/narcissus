#!/bin/bash

if [ $# -ne 2 ]; then
    echo for example
    echo ./laod.sh /dev/pts/14 osbook_03/08/os/kozos.elf
    exit 1
fi

echo load
python -c "print('load\n')" > ${1}

echo sx
sx ${2} > ${1} < ${1}

# echo run
# python -c "print('run\n')" > ${1}

