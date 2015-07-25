# Build utilites for R3X
export CC=gcc
cd disasm
./build.sh
mv ./readrex $PREFIX
mv ./rexdump $PREFIX
mv ./cgen $PREFIX
cd ..
cd compiler
$CC -std=gnu99 -g tbc.c -o tbc
$CC -std=gnu99 t++.c -o t++ -D RX_PREFIX="\"$PREFIX\""
mv ./tbc $PREFIX
mv ./t++ $PREFIX 
cd ../src
cd programs
cp -r ./libR3X/ $PREFIX
cd .. 
cd ..
cd $PREFIX
mkdir -p include
cp -r ../src/lib/rstdlib/include/rstdlib.h ./include/
export STDLIB_R3X="$PREFIX/include"
