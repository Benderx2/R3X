echo "REX Compilation Script, version 0.56a"
set -o verbose
# Change to x86_64, x86_32, aarch64, aarch64-big,ppc depending upon stuff..
export TARGET="x86_64"
# Set to empty if compiling for other arch without dynamic linking support or graphics
export USEGL="yes"
export USEDYNAMIC="yes"
# Change to -O3 for more optimization or -O0 for no optimization
export OFLAGS="-O3"
# Change to empty if you don't want debugging information with the binary
export DFLAGS="-g"
# DONT MODIFY 
export AS="fasm"
export GLFLAGS=""
export GL_FILES=""
export DYNAMICFLAGS=""
export DYANMIC_FILES=""
export ARFLAGS="-rsc"
export INCLUDE_DIR="./include/"  
export CFLAGS="-std=gnu99 -Wall -Wextra -Wstrict-aliasing -Wstrict-prototypes -Wmissing-prototypes -fstack-protector-all -fomit-frame-pointer"
export IFLAGS="-I$INCLUDE_DIR"
if [ "$USEGL" == "yes" ] 
	then 
	 export USEGL="-D REX_GRAPHICS"
	 export GLFLAGS="-lSDL -lSDL_image -lGL -lX11"
	 export GL_FILES="r3x_font.o r3x_graphics.o"
fi
if [ "$USEDYNAMIC" == "yes" ]
	then
	 export USEDYNAMIC="-D REX_DYNAMIC"
	 export DYNAMICFLAGS="-ldl"
	 export DYNAMIC_FILES="r3x_native.o"
fi
if [ "$TARGET" == "x86_64" ]
	then
	 export ARCHFLAGS="-m64" 
	 export ENDIANFLAGS="-D R3X_LITTLE_ENDIAN" 
         export ARCHID="-D LINUX_ARCH_X8664"
	 export CC="gcc" 
	 export AR="ar"
	 export BINDIR="../bin64"
elif [ "$TARGET" == "x86" ]
	then
	 export ARCHFLAGS="-m32"
	 export ENDIANFLAGS="-D R3X_LITTLE_ENDIAN"
	 export ARCHID="-D LINUX_ARCH_X8632"
	 export CC="gcc"
	 export AR="ar"
	 export BINDIR="../bin32"
elif [ "$TARGET" == "aarch64-big" ]
	then 
         # Don't use unless you've got big-endian target libraries..
	 export ARCHFLAGS="-mbig-endian"
	 export ENDIANFLAGS="-D R3X_BIG_ENDIAN"
	 export ARCHID="-D LINUX_ARCH_ARM64"
	 export CC="arm-linux-gnueabi-gcc"
	 export AR="arm-linux-gnueabi-ar"
	 export BINDIR="../binAARCH64" 
elif [ "$TARGET" == "aarch64" ]
	then 
	 export ARCHFLAGS=""
	 export ENDIANFLAGS="-D R3X_LITTLE_ENDIAN"
	 export ARCHID="-D LINUX_ARCH_ARM64"
	 export CC="arm-linux-gnueabi-gcc"
	 export AR="arm-linux-gnueabi-ar"
	 export BINDIR="../binAARCH64" 
elif [ "$TARGET" == "ppc" ]
	then
	 export ARCHFLAGS=""
	 export ENDIANFLAGS="-D R3X_BIG_ENDIAN"
	 export ARCHID="-D LINUX_ARCH_PPC"
	 export CC="powerpc-linux-gnu-gcc -static"
	 export AR="powerpc-linux-gnu-ar"
	 export BINDIR="../binppc" 
else 
 	echo "What the fuck? Unknown or unsupported target architecture specified"
	exit -1
fi
export LFLAGS="-lc -lm -ldl -rdynamic"
export LINKER_FILES="r3x_cpu.o r3x_object.o r3x_main.o r3x_bios.o r3x_format.o r3x_exception.o r3x_stack.o  r3x_dispatcher.o r3x_dynamic.o r3x_stream.o libntmalloc.a"
# Compile libntmalloc
$CC -c ../libntmalloc/nt_malloc.c -o nt_malloc.o -std=gnu99
$AR  $ARFLAGS libntmalloc.a nt_malloc.o
export CCFLAGS="$ARCHID $USEDYNAMIC $USEGL $ENDIANFLAGS $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS"
# compile VM
for i in *.c
do
   set -x
   $CC $CCFLAGS -c $i -o ${i%.c}.o
   set +x
done
$CC -o r3x_vm.out $LINKER_FILES $GL_FILES $DYNAMIC_FILES $LFLAGS $GLFLAGS
# compile programs
$AS programs/r3x_ex.asm
$AS programs/math.asm
$AS programs/stream.asm
$AS programs/simplelib.asm
$AS programs/bios.asm
$AS programs/exception.asm
# now transfer it 
mv r3x_vm.out $BINDIR/
mv programs/r3x_ex.exe $BINDIR/
mv programs/math.exe $BINDIR/
mv programs/stream.exe $BINDIR/
mv programs/exception.exe $BINDIR/
mv programs/simplelib.ro $BINDIR/
mv programs/bios.bin $BINDIR/bios
# remove all object files
rm *.o
