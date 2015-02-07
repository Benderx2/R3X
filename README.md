R3X
===
![Alt text](http://i.imgur.com/Klw1O26.png)<br>
Official repository for the R3X (REX) virtual machine. See docs/REX.html for more details.<br>
![Alt text]http://i.imgur.com/c97xG9g.png "R3X Running on Linux64 with example program and debugger")
<h1>What is REX?</h1>
R3X is a runtime environment for portable applications. it has currently been ported to x86, x86-64, PPC, and ARM architectures.<br>
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
All documentation is in HTML format under the docs/ directory.<br>
Current version is 0.4.6a BASICPie (unstable)<br>
