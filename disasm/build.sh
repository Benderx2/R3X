export CC='gcc'
export CFLAGS='-Wall -Wextra -std=gnu99'
$CC $CFLAGS rexdump.c -o rexdump
$CC $CFLAGS readrex.c -o readrex