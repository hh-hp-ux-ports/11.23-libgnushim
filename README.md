# libgnushim — gnulib compatibility shim library for HP-UX 11.23 (ia64)

Static library supplying the libc functions HP-UX 11.23 lacks, so modern GNU
sources link without per-package fights. Built from a pinned 2020-vintage
gnulib (modern gnulib's header wrappers are broken on 11.23 — proven with
sed 4.9 / coreutils 9.11).

## Functions provided (all VERIFIED missing from 11.23 libc by link-probe, see probe/)
setenv unsetenv getline getdelim strnlen strcasestr mkstemps futimens
canonicalize_file_name getopt_long (GNU getopt) strndup stpcpy stpncpy
vasprintf asprintf strverscmp mempcpy

Confirmed PRESENT in 11.23 (not shimmed): glob, globfree, isblank, iswblank, wcwidth.
Driving use case: GNU nano 9.1 (its gnulib-cache module list) + general toolchain builds.

## Consumers
    #include <gnushim.h>                       /* /opt/gnu/include */
    cc ... -L/opt/gnu/lib -lgnushim            # ILP32
    cc -mlp64 ... -L/opt/gnu/lib/hpux64 -lgnushim   # LP64
Ships in the GNUcombined depot: GNUtools.dev fileset.

## Layout
- `gnushim.h` — consumer header (installed to /opt/gnu/include)
- `modules.txt` — the gnulib module list (one per line)
- `regenerate.sh` — dev box: re-create `testdir/` from /mnt/nfs/gnulib-2020 (pinned checkout 37b6f12946, 2020-08-01)
- `build_rx2620.sh` — rx2620: build both ABIs, nm-verify every symbol, run gnulib self-tests, stage
- `probe/probe.sh` — the libc-gap probe battery (documents WHY each function is in/out)
- `testdir/` — the generated gnulib source tree (committed for reproducibility without network)

## History
- v1 (2026-07-22): setenv/unsetenv only
- v2 (2026-07-22): nano-driven expansion to 17 functions, 2020-vintage gnulib, git-tracked
