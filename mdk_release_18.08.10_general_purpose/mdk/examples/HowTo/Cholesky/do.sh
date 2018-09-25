#! /bin/sh
N="$1"
if [ "z$N" = "z" ]; then
  echo Use: $0 \<matrix size\>
  return 1 2>/dev/null
  exit
fi
if test ! -e vectors/A_${N}x${N}.f32 -o ! -e vectors/M_${N}x${N}.f32 -o ! -e vectors/L_${N}x${N}.f32 -o \
        ! -e vectors/b_${N}.f32 -o ! -e vectors/c_${N}.f32 -o ! -e vectors/y_${N}.f32 -o ! -e vectors/x_${N}.f32
then
  echo Generating testcase data for ${N}x${N}...
  mkdir -p vectors
  test ! -x tools/solver && gcc -O3 -o tools/solver tools/solver.c tools/lib/core.c tools/lib/helpers.c -lm
  if ! tools/solver -q -n ${N} -g -gi -noGJ
  then
    return 1 2>/dev/null
    exit
  fi
  mv -f *_${N}x${N}.f32 *_${N}.f32 vectors/
fi
# patch the relevant files...
chmod 644 leon/main.c
# ...but only if needed
CRTN=`cat leon/main.c | grep "#define N ${N}"`
if test -z "${CRTN}"
then
  sed -ri "s|#define N[^\n]+|#define N ${N}|" leon/main.c
fi
mkdir -p scripts
chmod 644 -f scripts/debug_run_mdbg2.scr
echo "puts \"Loading application...\"" > scripts/debug_run_mdbg2.scr
echo "mdbg::loadfile -breset -ddrinit -elf output/Cholesky.elf" >> scripts/debug_run_mdbg2.scr
echo "puts \"Loading input data...\"" >> scripts/debug_run_mdbg2.scr
echo "mdbg::loadfile -binary -address A vectors/A_${N}x${N}.f32" >> scripts/debug_run_mdbg2.scr
echo "mdbg::loadfile -binary -address b vectors/b_${N}.f32" >> scripts/debug_run_mdbg2.scr
echo "puts \"Running...\"" >> scripts/debug_run_mdbg2.scr
echo "mdbg::runw" >> scripts/debug_run_mdbg2.scr
echo "puts \"Saving output+intermediate data...\"" >> scripts/debug_run_mdbg2.scr
echo "mdbg::savefile output/M_${N}x${N}.f32 bak_M" >> scripts/debug_run_mdbg2.scr
echo "mdbg::savefile output/c_${N}.f32     bak_c"  >> scripts/debug_run_mdbg2.scr
echo "mdbg::savefile output/L_${N}x${N}.f32 A"      >> scripts/debug_run_mdbg2.scr
echo "mdbg::savefile output/y_${N}.f32     bak_y"  >> scripts/debug_run_mdbg2.scr
echo "mdbg::savefile output/x_${N}.f32     x"      >> scripts/debug_run_mdbg2.scr
echo "puts \"All done.\"" >> scripts/debug_run_mdbg2.scr
chmod 644 scripts/debug_run_mdbg2.scr # cygwin workaround
# build
echo
echo Building...
chmod 644 -rf leon/*.o output/* shave/*.asmgen shave/*.o shave/*.shv?lib shave/*.mvlib 2> /dev/null
( make all $2 2>&1 || rm -f output/Cholesky.elf )| tee build-${N}x${N}.log
chmod 644 -rf leon/*.o output/* shave/*.asmgen shave/*.o shave/*.shv?lib shave/*.mvlib 2> /dev/null
if [ ! -f output/Cholesky.elf ]
then
  return 1
  exit
fi
# sparc-myriad-elf-strip -g output/Cholesky.elf # don't even think of doing this with moviDebug2!
# run
echo
echo Running...
rm -f output/run.success
( make run $2 2>&1 && touch output/run.success )| tee run-${N}x${N}.log
rm -f scripts/debug_run_mdbg2.scr
if [ ! -f output/run.success ]
then
  return 1 2>/dev/null
  exit
fi
rm -f output/run.success
# diff
test ! -x tools/matdiff && gcc -O3 -o tools/matdiff tools/matdiff.c -lm
echo
echo Comparing matrices \(N=$N\)...
echo -n "  M: "
tools/matdiff -lt output/M_${N}x${N}.f32 vectors/M_${N}x${N}.f32
echo -n "  c: "
tools/matdiff -vec output/c_${N}.f32 vectors/c_${N}.f32
echo -n "  L: "
tools/matdiff -lt output/L_${N}x${N}.f32 vectors/L_${N}x${N}.f32
echo -n "  y: "
tools/matdiff -vec output/y_${N}.f32 vectors/y_${N}.f32
echo -n "  x: "
tools/matdiff -vec output/x_${N}.f32 vectors/x_${N}.f32
echo
echo Timings \(N=$N\):
cat run-${N}x${N}.log | egrep '^UART:  ' | grep ms | sed 's/^UART:   - /  /g'
echo
echo All done.
