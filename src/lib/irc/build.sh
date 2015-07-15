gcc -I../include -g -std=gnu99 -c -Wall -Werror -fpic host.c -o host.o
gcc -g -shared -o host.so ./host.o ../rxvmlib.a ../libntmalloc.a -Wl,-no-whole-archive 
t++ -s ./ircbot.bas -o ircbot.exe
rm *.o
