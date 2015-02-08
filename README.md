R3X
===
![Alt text](http://i.imgur.com/Klw1O26.png)<br>
Official repository for the R3X (REX) virtual machine. See docs/REX.html for more details.<br>
![Alt text](http://i.imgur.com/c97xG9g.png "R3X Running on Linux64 with example program and debugger")
<h1>What is REX?</h1>
R3X is a runtime environment for portable applications. it has currently been ported to x86, x86-64, PPC, and ARM architectures and runs on Linux as well as Windows.<br>
<br>

R3X programs are compiled to it's own bytecode, which can be run using the VM and are architecture independent. The main goal for R3X is to create an environment which offers maximum portability, while ensuring performance and features.
R3X aims to provide a C-like enviroment, at the same time being highly portable and safe. It currently supports streams, exception handling, multithreading, dynamic memory allocation and dynamic linking
The VM also comes with an inbuilt debugger. <br>

<h1>What languages are supported by the runtime? </h1>
<br>

Currently the only supported language is a dialect of BASIC, which supports function calls, inline assembly, bitwise and integer arithmetic, and probably lots to come! Of course, an LLVM backend for R3X is being
worked on, which will make it possible for the clang toolchain to generate code for R3X. <br>

<h1>Does the runtime use tricks to speed up code (architecture specific code generator? optimizer? etc..)</h1>

Unfortunately, R3X code is totally interpreted, but yes, a just-in-time compiler for x86_64 is under development.<br>

<h1>Any possible documentation available?</h1>
<br>
All documentation is in HTML format under the docs/ directory.<br>

<h1>How 2 Buildz?</h1>
<br>
Building the virtual machine can be done using a bash-compatible shell (bash on linux, and msys on Windows), for Linux systems simply run: <br> 
`./build.sh arch=x86_64` (See docs/Building.html for building with a different architectures)<br>
Unfortunately, Windows x86_64 is only supported as of now, recommended options to run ./build.sh are:<br>
`./build.sh arch=x86_64win usegl=no usedynamic=no`<br>
In order to use native libraries (usedynamic=yes) for Windows, add an implementation of "dlfcn.h" into your headers. An existing implementation can be found here: https://github.com/dlfcn-win32/dlfcn-win32
OpenGL support for Windows is currently untested.
In order to build the utilities, run "buildutils.sh" given in the root directory of the project. This will build the compiler front end, the preprocessor, rexdump, and the disassembler and transfer them into
the "binutils" direcotry

<h1>Is OS X supported</h1>
Uh, no :D. Not because I don't like OS X, but because I don't have an OS X machine to test stuff on. I'd be glad if somebody experienced with OS X APIs, could point out what might be needed to port to OS X.
To know what porting the VM requires, please have a look at: "docs/Porting.html" for the parts of the VM that may need changes.

<h1>Bugs n Contributing?</h1>
For bugs, just push them to the issues section, and as for contribs... <br>
lolwut? If you seriously want to contribute, please don't dump code, wait contributing is currently not supported.<br>

<h1>Reason?</h1>
R3X doesn't have a CLA (Contributor License Agreement), which defines your (the contributor) fucking limits.<br>

<h1>k, how do I code for this virtual machine?</h1>

You can use FASM (flat assembler), for writing assembly code, no, it's not x86, you have to include the assembly header in 
"src/programs/include/libR3X.pkg", which defines macros for instructions. You can then use R3X instructions, which will be
compiled to R3X bytecode. For instructions supported see "docs/CPU.html and docs/Opcodes.html".<br>
...<br>
Or you can use the tinyBASIC compiler to generate R3X assembly code and then compile it using fasm. (you'll need the assembly header).
<br>
Current version is 0.4.6a BASICPie (unstable)<br>
