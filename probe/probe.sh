#!/usr/bin/sh
# probe.sh — RX2620: link-probe which candidate functions exist in this libc (LP64).
# Output = the ground truth for what belongs in libgnushim. Last run 2026-07-22: see README.
export PATH=/opt/gcc474/bin:/usr/ccs/bin:/usr/bin:/bin
cd /var/tmp
for f in getline getdelim strnlen strcasestr mkstemps futimens isblank iswblank wcwidth \
         canonicalize_file_name glob globfree getopt_long strndup stpcpy stpncpy vasprintf \
         asprintf setenv unsetenv strverscmp mempcpy; do
  cat > p$$.c <<PEOF
char $f ();
int main (void) { return $f (); }
PEOF
  if gcc -mlp64 p$$.c -o p$$.x 2>/dev/null; then echo "PRESENT $f"; else echo "MISSING $f"; fi
done
rm -f p$$.c p$$.x
