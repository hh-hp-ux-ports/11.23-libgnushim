#!/bin/sh
# regenerate.sh — DEV BOX: rebuild testdir/ from the pinned 2020-vintage gnulib checkout.
# Requires autoconf/automake installed (apt). Pinned: /mnt/nfs/gnulib-2020 @ 37b6f12946 (2020-08-01).
set -e
cd "$(dirname "$0")"
rm -rf testdir.new
/mnt/nfs/gnulib-2020/gnulib-tool --create-testdir --dir=testdir.new --lib=libgnu $(cat modules.txt)
rm -rf testdir && mv testdir.new testdir
chmod -R a+rX testdir
echo "testdir regenerated"
