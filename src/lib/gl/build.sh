gcc -I../include -std=gnu99 -c -Wall -Werror -fpic glExec.c -o glExec.o
gcc -shared -o glExec.so ./glExec.o ../rxvmlib.a ../libntmalloc.a -lc -lm -lSDL -lSDL_image -lGL -lX11 -Wl,-no-whole-archive 
rm *.o
