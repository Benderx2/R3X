export CC="gcc"
export CFLAGS="-Wall -Wextra -std=gnu99"
$CC $CFLAGS devenv.c libntmalloc.a -o devenv.out
mv ../bin64/
