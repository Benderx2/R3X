This is the directory where you would find a stripped down version of the Smaller C compiler's repository written
and maintained by Alexey Frunze. The original repository comes with a frontend, library, linker and lots of 
examples. Please do not use this directory as a substitute to the original one. This is maintained
ONLY for REX systems, and may be updated less frequently than the original one.

Please find the original Smaller C repository here: https://github.com/alexfru/SmallerC/tree/master.

This directory contains 5 files:
license.txt - Original Smaller C license.
smlrc.c - Compiler core (Frontend not included for obvious reasons)
cgx86.c - Compiler code generator for x86 Systems.
cgmips.c - Compiler code generator for MIPS Systems.
cgr3x.c - (In progress, maintained by me) Compiler code generator for R3X systems.

Please note that currently the compiler is only added for future reference, and the 
code generator for R3X systems is in very early stages. I'm planning to finish it
within the next 2 weeks or so but I (unfortunately) cannot guarantee it because of my
lack of time available (Calculus with Physics ain't fun ;P ). 
