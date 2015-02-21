R3X
===
![Alt text](http://i.imgur.com/Klw1O26.png)<br>
Official repository for the R3X (REX) virtual machine. See docs/REX.html for more details.<br>
![Alt text](http://i.imgur.com/c97xG9g.png "R3X Running on Linux64 with example program and debugger")
![Alt text](http://i.imgur.com/wcPCVSP.png "R3X running a program written in rbasic")
<h1>What is R3X?</h1>
R3X is a runtime environment for portable applications. It is similar to JVM (Java Virtual Machine) or the more famous .NET CLR (Common Language Runtime)
it has currently been ported to x86, x86-64, PPC, and ARM architectures and runs on Linux as well as Windows.<br>
<br>

R3X programs are compiled to an architecture independent format, which can be run using the VM 
and are architecture independent. The main goal for R3X is to create an environment which offers maximum portability,
while ensuring performance and features. It currently supports I/O, exception handling, multithreading, and dynamic linking.
The runtime also comes with an inbuilt debugger, which has an integrated disassembler, stack tracing, 
and a lot of nifty features. <br>

<h1>What languages are supported by the runtime? </h1>
The runtime currently has it's own programming language called "T++", (see the /compiler directory), the compiler is in a usable state<br>
and can compile useful programs.<br>
Of course, an LLVM backend is being developed for R3X, which will make it possible for the clang toolchain to generate code for the runtime.<br>

<h1>Does the runtime use tricks to speed up code (architecture specific code generator? optimizer? etc..)</h1>
Unfortunately, R3X code is totally interpreted, but yes, a just-in-time compiler for x86_64 is under development.<br>

<h1>Any possible documentation available?</h1>
All documentation is in HTML format under the docs/ directory.<br>

<h1>Building</h1>
Building the virtual machine can be done using a bash-compatible shell (bash on linux, and msys on Windows), for Linux systems simply run: <br> 
`./build.sh arch=x86_64` (See docs/Building.html for building with a different architectures)
Unfortunately, Windows x86_64 is only supported as of now, recommended options to run ./build.sh are:<br>
`./build.sh arch=x86_64win usegl=no usedynamic=yes`<br>
OpenGL support for Windows is currently untested.
In order to build the utilities, run "buildutils.sh" given in the root directory of the project. This will build the compiler front end, the preprocessor, rexdump, and the disassembler and transfer them into
the "binutils" directory.
<h1>Running?</h1>
See "docs/Building.html", for a list of options which can be given to "rxvm".

<h1>Writing programs for R3X</h1>

You can use FASM (flat assembler), for writing assembly code, no, it's not x86, you have to include the assembly header in 
"src/programs/include/libR3X.pkg", which defines macros for instructions. You can then use R3X instructions, which will be
compiled to R3X bytecode. For instructions supported see "docs/CPU.html and docs/Opcodes.html".<br>
...<br>
Or you can use the tinyBASIC compiler to generate R3X assembly code and then compile it using fasm. (you'll need the assembly header).

<h1>Possible set of features?</h1>
* File I/O
* Multithreading
* Exceptions
* Memory Protection
* Dynamic Linking
* Native Object Linking (Programs can call procedures in .so or .dll files)
* High Level Programming language (T++)
* Inbuilt debugger
* A relatively well-featured toolchain (disassembler, executable reader, compiler)
* A server-client-like implementation. (The server has the ability to run client VMs concurrently)
... and lots to come!
<br>
Current version is 0.4.6b BASICPie (unstable)<br>
