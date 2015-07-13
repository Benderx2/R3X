gcc -I../include -std=gnu99 -c -Wall -Werror -fpic host.c -o host.o
gcc -shared -o host.so ./host.o ../rxvmlib.a ../libntmalloc.a -Wl,-no-whole-archive 
t++ -s ./ircbot.bas -o ircbot.exe
rm *.o
