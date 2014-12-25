#!/bin/bash
echo "REX Compilation Script, version 0.03"
export ARFLAGS="-rsc"
export INCLUDE_DIR="./include/"
export ARCHFLAGS="-m64" # Change to -m32 for 32-bit
export CFLAGS="-std=gnu99 -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -fstack-protector-all"
export IFLAGS="-I$INCLUDE_DIR"
export OFLAGS="-O2"
export DFLAGS="-g"
export LFLAGS="-lc -lm -lSDL -lSDL_ttf -lSDL_image -ldl -lGL -rdynamic"
export LINKER_FILES="r3x_cpu.o r3x_object.o r3x_font.o r3x_main.o r3x_bios.o r3x_format.o r3x_exception.o r3x_native.o r3x_stack.o r3x_graphics.o r3x_dispatcher.o libntmalloc.a"
set -x
# Compile libntmalloc
gcc -m64 -c nt_malloc.c -o nt_malloc.o -std=gnu99 -I$INCLUDE_DIR
ar  $ARFLAGS libntmalloc.a nt_malloc.o
# compile VM
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_cpu.c -o r3x_cpu.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_object.c -o r3x_object.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_main.c -o r3x_main.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_font.c -o r3x_font.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_format.c -o r3x_format.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_native.c -o r3x_native.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_stack.c -o r3x_stack.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_graphics.c -o r3x_graphics.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_exception.c -o r3x_exception.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_bios.c -o r3x_bios.o
gcc $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS -c r3x_dispatcher.c -o r3x_dispatcher.o
gcc -o r3x_vm.out $LINKER_FILES $LFLAGS
# compile programs
fasm programs/r3x_ex.asm
fasm programs/bios.asm
# now transfer it 
mv r3x_vm.out ../bin/
mv programs/r3x_ex.bin ../bin
mv programs/bios.bin ../bin/bios
mv *.a ../bin/lib/
# remove all object files
rm *.o
set +x
