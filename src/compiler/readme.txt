WARNING
[
THIS MANUAL CONTAINS "BAD WORDS"! yes, the ones you little sh#$)*@)*@!s refer to as "effyouseekay".
Go back to your mom/dad and ask them to engage you into something better.

plz dun see if you dun want to get offended.

liek if you cry ev4erytime.
]

TBC+ - TinyBasic+ for R3X
TBC+ is a small extended version of the TinyBASIC programming language. It is designed to run
on R3X systems.
I'd like to give credit to the original creator by providing the link to the original repository: https://github.com/Luiji/TinyBASIC-C
Language:
	TBC+ is very similar to TinyBASIC and other BASIC-based languages, however there are certain differences.
	
Keywords:
	<INT> currently is a single ASCII-char [A-Z] only!
	
	PRINT "STR"; INT - Print a string to the terminal, or an integer as described.
	INPUT <INT> - Input an integer from the terminal
	ALLOC <INT> = EXPRESSION - Allocate bytes of memory given by expression and store address in <INT>.
	LET [A-Z] = EXPRESSION - Set a variable (A-Z Only), to the value of EXPRESSION
	PTR [A-Z] = EXPRESSION - Set a variable (A-Z Only). to the 32-bit value at the addressof(value of EXPRESSION)
	ASM "<asm code>" - Output the assembler code directly
	IF [A-Z] [> < = >= <=] INT/LITERAL GOTO/GOTOL INT/LABEL - Jump to a line number / label if condition is true.
	LABEL labelname - Declare a label
	GOTO - Goto a line number
	GOTOL - Goto a label
	GOSUB - Goto a line number, and push return address to stack
	GOSUBL - Goto a label and push address to stack
	RETURN - Return to the address pushed to stack
	WHILE EXPRESSION [> < = >= <=] EXPRESSION - Do a while loop (max 256 nestings allowed)
	ENDW - End while
Arrays:
	Currently array support is limited, yes it is, fuck off.
	Creating an array:
		ALLOC S = 256 --- Allocate 256 bytes for S.
		[S] = 'A'
		[S+1] = 0
		LET X = [S] // X = 'A' or 65.
Operators:
	Current supported operators are +, -, /, *, &, ^, |. There is operator precendence, but it's very shitty.
	Please don't use it, instead just use fkn parentheses, and if you can't then go fuck yourself, I don't
	have time for this shit.
	
	Thank you. And fuck off, please goto the second last line of this page to see what it means.

Using variables in inline asm:
	The address to variables can be referred to as v0 [for variable A]. v1 [for variable B] and so on..
	You're free to use these addresses to modify variables in inline asm. Yes, you're free the fuck with
	whatever you want, just dont come to me.

Usage:
	./tbc file.bas > output.asm
	fasm output.asm
P.S. While compiling with FASM you will need an 'libR3X.pkg' and 'r3x_asm.pkg' which are in src/programs/libR3X.

-----Uh, this document is badly written, I'll write a better version when I've more time on my hands.-----

Please goto line 39.
... OMFG WTF RECURSION??!?!?! RECURSION IS SUPPORTED YAY!
