#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/gregcsl/Desktop/scalehls/tools/pyscalehls/generated_files/vhls_dse_temp/kernel_2mm_1643577861_17626/solution1/.autopilot/db/a.g.bc ${1+"$@"}
