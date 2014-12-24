include 'libR3X/libR3X.inc'
_init:
	Console.Write "Hello, World!"
	Console.NewLine
	Console.Write "This is the beta preview of R3X (read : REX) virtual machine"
	Console.NewLine
	Console.Write "Feel free to visit https://www.github.com/Benderx2/r3x for more information!"
	Console.NewLine
	Console.Write "Press a key to continue"
	Console.Input string, 5
	loadr R0, _cool
	pushar R0
	ret
_cool:
	Console.Write "Other func"
	exit
_end_text:	
_data:
	string: times 256 db 0
_end_data:
_bss:
_end_bss:
