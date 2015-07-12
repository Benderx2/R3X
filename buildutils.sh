# Build utilites for R3X
cd disasm
./build.sh
mv ./readrex $PREFIX
mv ./rexdump $PREFIX
mv ./cgen $PREFIX
cd ..
cd compiler
gcc -g tbc.c -o tbc
gcc -std=gnu99 t++.c -o t++ -D RX_PREFIX="\"$PREFIX\""
mv ./tbc $PREFIX
mv ./t++ $PREFIX 
cd ../src
cd programs
cp -r ./libR3X/ $PREFIX
cd ..
cd ..
