CC=gcc
AR=ar
AS=fasm
ARCHFLAGS=$(shell getconf LONG_BIT)
CFLAGS=-m$(ARCHFLAGS) -std=gnu99 -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -fstack-protector-all -I ./include/ -c
LDFLAGS=libntmalloc.a -lc -lm -lSDL -lSDL_ttf -lSDL_image -ldl -lGL -rdynamic
SOURCES=r3x_main.c r3x_graphics.c r3x_object.c r3x_cpu.c r3x_font.c r3x_format.c r3x_native.c r3x_stack.c r3x_exception.c r3x_bios.c r3x_dispatcher.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=r3x_vm.out
fresh: ntmalloc all examples clean

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(ARCHFLAGS) $(CFLAGS) $< -o $@

ntmalloc: 
	$(CC) -m64 -I ./include/ -std=gnu99 -c nt_malloc.c -o nt_malloc.o
	$(AR) -rsc libntmalloc.a nt_malloc.o

examples:
	$(AS) ./programs/r3x_ex.asm 
	$(AS) ./programs/bios.asm

clean:
	rm -rf *.o
	mv r3x_vm.out ../bin$(ARCHFLAGS)/
	mv ./programs/r3x_ex.bin ../bin$(ARCHFLAGS)
	mv ./programs/bios.bin ../bin$(ARCHFLAGS)/bios
	mv *.a ../bin$(ARCHFLAGS)/lib/
