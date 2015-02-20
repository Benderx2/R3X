export CC='gcc'
export CFLAGS='-Wall -Wextra -std=gnu99 -I../src/include'
$CC $CFLAGS rexdump.c -o rexdump
$CC $CFLAGS readrex.c -o readrex
$CC $CFLAGS cgen.c -o cgen
