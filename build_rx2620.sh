#!/usr/bin/sh
# build_rx2620.sh — RX2620: build libgnushim v2 from testdir/, BOTH ABIs, nm-verify every promised
# symbol, run the gnulib self-tests (never skipped), stage into the GNUtools staging tree.
BUILD=/home/claude/build
REPO=/mnt/debianshare/libgnushim
STAGE=$BUILD/shimext-stage          # the GNUcombined packaging source for /opt/gnu
LOG=$BUILD/gnushim2.log ; DONE=$BUILD/gnushim2.DONE
exec > $LOG 2>&1
STATUS=running
on_exit(){ rc=$?; echo "status=$STATUS rc=$rc when=`date`" > "$DONE"; cp "$DONE" /mnt/debianshare/gnu_tarballs/driver_status/gnushim2.DONE 2>/dev/null; }
trap on_exit 0 ; trap 'STATUS=killed; exit 130' 1 2 3 15
say(){ echo "@@@ $1 :: `date`"; }
fail(){ say "FAILED: $1"; STATUS="FAILED $1"; exit 1; }
PATH=/opt/gcc474/bin:/opt/binutils/bin:/opt/gnu/bin:/usr/ccs/bin:/usr/bin:/bin ; export PATH
SYMS="setenv unsetenv getline getdelim strnlen strndup strcasestr stpcpy stpncpy strverscmp mempcpy asprintf vasprintf mkstemps futimens canonicalize_file_name getopt_long"
[ -f $REPO/testdir/configure ] || fail "no testdir/configure (run regenerate.sh on dev box)"
for ABI in 32 64 ; do
  case $ABI in 32) MF="" ; LIBD=$STAGE/opt/gnu/lib ;; 64) MF="-mlp64" ; LIBD=$STAGE/opt/gnu/lib/hpux64 ;; esac
  OBJ=$BUILD/gnushim2-obj-$ABI
  say "===== ABI $ABI configure ====="
  rm -rf $OBJ ; mkdir -p $OBJ ; cd $OBJ || fail cd
  $REPO/testdir/configure CC="gcc $MF" CFLAGS="-O2" > cfg.log 2>&1 || { tail -20 cfg.log ; fail "configure abi$ABI" ; }
  gmake > mk.log 2>&1 || { tail -30 mk.log ; fail "build abi$ABI" ; }
  [ -f gllib/libgnu.a ] || fail "no libgnu.a abi$ABI"
  # gnulib exports SOME functions under its rpl_ prefix (mixed, per-function, per-ABI). Detect
  # exactly which, alias those to plain names (aliases.c), and add our own getopt_long (gnulib's
  # is rpl_-namespaced WITH its own rpl_optarg globals — aliasing it would desync libc optarg).
  say "ABI $ABI alias detection + augmentation"
  DEFS=""
  for s in $SYMS ; do
    if nm gllib/libgnu.a 2>/dev/null | awk "/\|${s}\$/ && /FUNC/ && /GLOB/ && !/UNDEF/" | grep . >/dev/null ; then
      : # plain symbol present
    elif nm gllib/libgnu.a 2>/dev/null | awk "/\|rpl_${s}\$/ && /FUNC/ && /GLOB/ && !/UNDEF/" | grep . >/dev/null ; then
      U=`echo $s | tr "[:lower:]" "[:upper:]"`
      DEFS="$DEFS -DNEED_ALIAS_$U"
    fi
  done
  say "ABI $ABI aliases needed:$DEFS"
  gcc $MF -O2 $DEFS -c /mnt/debianshare/libgnushim/aliases.c -o aliases.o || fail "aliases abi$ABI"
  gcc $MF -O2 -I/mnt/debianshare/libgnushim -c /mnt/debianshare/libgnushim/getopt_long.c -o getopt_long.o || fail "getopt_long abi$ABI"
  ar r gllib/libgnu.a aliases.o getopt_long.o || fail "ar augment abi$ABI"
  say "ABI $ABI symbol verification (post-augment)"
  MISSING=""
  for s in $SYMS ; do
    nm gllib/libgnu.a 2>/dev/null | awk "/\|${s}\$/ && /FUNC/ && /GLOB/ && !/UNDEF/" | grep . >/dev/null || MISSING="$MISSING $s"
  done
  [ -z "$MISSING" ] || { nm gllib/libgnu.a | grep -iE "getopt|getline|canonical" | head -10 ; fail "abi$ABI missing symbols:$MISSING" ; }
  say "ABI $ABI gnulib self-tests (full run)"
  gmake check > chk.log 2>&1
  say "ABI $ABI check exit=$?"
  grep -E "^(# TOTAL|# PASS|# FAIL|# SKIP|# ERROR)" chk.log | head -6
  mkdir -p $LIBD
  cp gllib/libgnu.a $LIBD/libgnushim.a || fail "stage abi$ABI"
done
cp $REPO/gnushim.h $STAGE/opt/gnu/include/gnushim.h || fail "stage header"
say "smoke: getline + strverscmp + getopt_long via installed-style link, both ABIs"
cat > /var/tmp/shim2t.c <<'TC'
#include "gnushim.h"
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  static const struct option lo[] = { {"alpha", required_argument, 0, 'a'}, {0,0,0,0} };
  int li = 0; char *line = NULL; size_t n = 0; FILE *f;
  (void)argc; (void)argv;
  if (strverscmp("v10", "v9") <= 0) return 1;
  f = fopen("/etc/PATH", "r"); if (!f) return 2;
  if (getline(&line, &n, f) <= 0) return 3;
  fclose(f); free(line);
  { char *av[4]; int c;
    av[0]=(char*)"t"; av[1]=(char*)"--alpha"; av[2]=(char*)"V"; av[3]=0;
    c = getopt_long(3, av, "a:", lo, &li);
    if (c != 'a' || !optarg || optarg[0] != 'V') return 4; }
  { char *p = strcasestr("Hello WORLD", "world"); if (!p) return 5; }
  puts("gnushim2-ok"); return 0;
}
TC
gcc        -I$STAGE/opt/gnu/include /var/tmp/shim2t.c -L$STAGE/opt/gnu/lib        -lgnushim -o /var/tmp/s2t32 && /var/tmp/s2t32 || fail "smoke ilp32"
gcc -mlp64 -I$STAGE/opt/gnu/include /var/tmp/shim2t.c -L$STAGE/opt/gnu/lib/hpux64 -lgnushim -o /var/tmp/s2t64 && /var/tmp/s2t64 || fail "smoke lp64"
STATUS="complete — libgnushim v2 (17 funcs) staged, both ABIs, smoke-run"
say "★ LIBGNUSHIM v2 COMPLETE"
