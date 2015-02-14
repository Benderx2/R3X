echo "REX Compilation Script, version 0.71b"
set -o verbose
if [ "$1" == "arch=x86_64" ]
then
	export TARGET="x86_64"
elif [ "$1" == "arch=x86_32" ]
then
	export TARGET="x86_32"
elif [ "$1" == "arch=aarch64" ]
then
	export TARGET="aarch64"
elif [ "$1" == "arch=aarch64-big" ]
then
	export TARGET="aarch64-big"
elif [ "$1" == "arch=ppc" ]
then
	export TARGET="ppc"
elif [ "$1" == "arch=x86_64win" ]
then
	export TARGET="x86_64win"
else
# Default to x86_64, x86_32, aarch64, aarch64-big,ppc depending upon stuff..
export TARGET="x86_64"
fi
# Check for GL Options
if [ "$2" == "usegl=yes" ]
then
	export USEGL="yes"
elif [ "$2" == "usegl=no" ]
then
	export USEGL=""
else 
	export USEGL="yes"
fi
# Check of dynamic options
if [ "$3" == "usedynamic=yes" ]
then
	export USEDYNAMIC="yes"
elif [ "$3" == "usedynamic=no" ]
then
	export USEDYNAMIC=""
else 
export USEDYNAMIC="yes"
fi
# Check for optimize options
if [ "$4" == "useoptimize=yes" ]
then
	export USEOPTIMIZE="yes"
elif [ "$4" == "useoptimize=no" ] 
then
	export USEOPTIMIZE=""
else 
	export USEOPTIMIZE="yes"
fi
# Change this to your preferred level of optimization
export OFLAGS="-O3 -fomit-frame-pointer -falign-functions -falign-loops -funsafe-math-optimizations -funsafe-loop-optimizations"
# Change to empty if you don't want debugging information with the binary
export DFLAGS=""
#export DFLAGS="-g -D R_DEBUG"
# DONT MODIFY
export AS="fasm"
export GLFLAGS=""
export GL_FILES=""
export DYNAMICFLAGS=""
export DYANMIC_FILES=""
export ARFLAGS="-rsc"
export INCLUDE_DIR="./include/"
export CFLAGS="-std=gnu99 -Wall -Wextra -Wstrict-aliasing -Wstrict-prototypes -Wmissing-prototypes "
export IFLAGS="-I$INCLUDE_DIR"
if [ "$USEGL" == "yes" ]
	then
	 export USEGL="-D REX_GRAPHICS"
	 export GLFLAGS="-lSDL -lSDL_image -lGL -lX11"
	 export GL_FILES="font.o graphics.o"
fi
if [ "$USEDYNAMIC" == "yes" ]
	then
	 export USEDYNAMIC="-D REX_DYNAMIC"
	 export DYNAMICFLAGS="-ldl -rdynamic"
	 export DYNAMIC_FILES="native.o"
fi
if [ "$USEOPTIMIZE" == "yes" ]
	then
	 export USEOPTIMIZE="-D REX_OPTIMIZE"
	 export OPTIMIZEFLAGS="-lpthread"
fi
if [ "$TARGET" == "x86_64" ]
	then
	 export ARCHFLAGS="-m64" 
	 export ENDIANFLAGS="-D R3X_LITTLE_ENDIAN" 
     export ARCHID="-D LINUX_ARCH_X8664"
	 export CC="gcc" 
	 export AR="ar"
	 export BINDIR="../bin64"
elif [ "$TARGET" == "x86_64win" ]
	then
	 export ARCHFLAGS="-m64"
	 export ENDIANFLAGS="-D R3X_LITTLE_ENDIAN"
	 export ARCHID="-DWIN_ARCH_X8664"
	 export CC="x86_64-w64-mingw32-gcc -static"
	 export AR="x86_64-w64-mingw32-ar"
	 export BINDIR="../bin64win"
elif [ "$TARGET" == "x86_32" ]
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
 	echo "Unsupported platform. Supported platforms: x86, x86_64, ppc, aarch64, aarch64-big"
	exit -1
fi
if [ "$TARGET" -ne "x86_64win" ] 
then
	export LFLAGS="-lc -lm"
else
	export LFLAGS="-lm"
fi
export LINKER_FILES="cpu.o object.o main.o bios.o format.o exception.o stack.o  dispatcher.o dynamic.o stream.o disassemble.o libntmalloc.a memory.o"
# Compile libntmalloc
$CC -c ../libntmalloc/nt_malloc.c -o nt_malloc.o -std=gnu99
$AR  $ARFLAGS libntmalloc.a nt_malloc.o
export CCFLAGS="$ARCHID $USEDYNAMIC $USEGL $USEOPTIMIZE $ENDIANFLAGS $ARCHFLAGS $CFLAGS $OFLAGS $DFLAGS $IFLAGS"
# compile VM
for i in *.c
do
   set -x
   $CC $CCFLAGS -c $i -o ${i%.c}.o
   set +x
done
$CC $ARCHFLAGS -o rxvm $LINKER_FILES $GL_FILES $DYNAMIC_FILES $LFLAGS $GLFLAGS $DYNAMICFLAGS $OPTIMIZEFLAGS
$CC -c -Wall -Werror -fpic ./programs/rex/stack.c -o vstack.o
$CC -c -Wall -Werror -fpic ./programs/mylib.c -o mylib.o
$CC -shared -o mylib.so ./mylib.o vstack.o -lc -lm -Wl,-no-whole-archive
# compile programs
$AS programs/r3x_ex.il 
$AS programs/math.il 
$AS programs/stream.il 
$AS programs/simplelib.il 
$AS programs/bios.il 
$AS programs/exception.il 
$AS programs/overflow.il
$AS programs/perf.il
$AS programs/stack.il
$AS programs/symbols.il
$AS programs/nativelib.il
$AS programs/jmp.il
$AS programs/hello.il
$AS programs/asshole.il
# now transfer it 
mv rxvm $BINDIR/
mv mylib.so $BINDIR/
mv programs/r3x_ex.exe $BINDIR/
mv programs/math.exe $BINDIR/
mv programs/stream.exe $BINDIR/
mv programs/exception.exe $BINDIR/
mv programs/overflow.exe $BINDIR/
mv programs/perf.exe $BINDIR/
mv programs/stack.exe $BINDIR/
mv programs/asshole.exe $BINDIR/
#mv programs/symbols.exe ../disasm/
mv programs/nativelib.exe $BINDIR/
mv programs/jmp.exe $BINDIR/
mv programs/hello.exe $BINDIR/
mv programs/simplelib.ro $BINDIR/
mv programs/bios.bin $BINDIR/bios
# remove all object files
rm *.o
# Remove static libs
rm *.a
