# Build utilites for R3X
cd disasm
./build.sh
mv ./readrex ../binutils
mv ./rexdump ../binutils
cd ..
cd src/compiler
gcc tbc.c -o tbc
gcc t++.c -o t++
mv ./tbc ../../binutils
mv ./t++ ../../binutils
cd ..
cd programs
cp -r ./libR3X/ ../../binutils
cd ..
cd ..
