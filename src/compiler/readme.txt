TBC+ - TinyBasic+ for R3X
TBC+ is a small extended version of the TinyBASIC programming language. It is designed to run
on R3X systems.
I'd like to give credit to the original creator by providing the link to the original repository: https://github.com/Luiji/TinyBASIC-C
Language:
	TBC+ is very similar to TinyBASIC and other BASIC-based languages, however there are certain differences.
	
Keywords:
	PRINT "STR"; INT - Print a string to the terminal, or an integer as described.
	INPUT <INT> - Input an integer from the terminal [Still being worked on]
	LET [A-Z] = EXPRESSION - Set a variable (A-Z Only), to the value of EXPRESSION
	PTR [A-Z] = EXPRESSION - Set a variable (A-Z Only). to the 32-bit value at the addressof(value of EXPRESSION)
	ASM "<asm code>" - Output the assembler code directly
	IF [A-Z] [> < = >= <=] INT/LITERAL GOTO/GOTOL INT/LABEL - Jump to a line number / label if condition is true.
	GOTO - Goto a line number
	GOTOL - Goto a label
	GOSUB - Goto a line number, and push return address to stack
	GOSUBL - Goto a label and push address to stack
	RETURN - Return to the address pushed to stack
	
Operators:
	Unfortunately, the current supported operators are +/-/"/"/*
	Support for bitwise operators |, &, ^ is being worked on.

Using variables in inline asm:
	The address to variables can be referred to as v0 [for variable A]. v1 [for variable B] and so on..
	You're free to use these addresses to modify variables in inline asm.

Usage:
	./tbc file.bas > output.asm
	fasm output.asm
P.S. While compiling with FASM you will need an 'libR3X.pkg' and 'r3x_asm.pkg' which are in src/programs/libR3X.
