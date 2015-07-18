gcc -I../include -std=gnu99 -c -Wall -Werror -fpic rstdlib.c -o rstdlib.o -O3
gcc -I../include -std=gnu99 -c -Wall -Werror -fpic rmath.c -o rmath.o -O3
gcc -I../include -std=gnu99 -c -Wall -Werror -fpic rsocket.c -o rsocket.o -O3
gcc -shared -o rstdlib.so ./rsocket.o ./rmath.o ./rstdlib.o ../rxvmlib.a ../libntmalloc.a -lc -lm -Wl,-no-whole-archive 
mv ./rstdlib.so ../../../bin64
rm *.o
