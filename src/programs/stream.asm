include 'libR3X/libR3X.inc'
_init:
	pushstring "./myfile.txt"
	syscall SYSCALL_OPENSTREAM
	push mybuf
	push 255
	syscall SYSCALL_READSTREAM
	pop
	pop
	pop
	syscall SYSCALL_CLOSESTREAM
	Console.Write "Read 255 bytes of ./myfile.txt: (Press a key to quit)"
	Console.NewLine
	Console.WritePointer mybuf
	Console.WaitKey
	exit
_end_text:
_data:
	mybuf: times 256 db 0
	STORE_SECTION_DATA
_end_data:
_bss:
	STORE_SECTION_BSS
_end_bss:
