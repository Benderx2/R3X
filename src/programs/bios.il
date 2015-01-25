; R3X System BIOS
; Configures initial system information
; The BIOS has no format, so just directly include the header..
include 'libR3X/r3x_asm.pkg'
define MACHINE_VER 0x36B
define BIOS_VER 007
define IS_UNIX 1
init:
	; Initialize all registers to 0
	loadr R1, 0
	loadr R2, 0
	loadr R3, 0
	loadr R4, 0
	loadr R5, 0
	loadr R6, 0
	loadr R7, 0
	loadr R8, 0
	loadr R9, 0
	loadr R10, 0
	loadr R11, 0
	loadr R12, 0
	loadr R13, 0
	loadr R14, 0
	loadr R15, 0
	loadr R16, 0
	loadr R17, 0
	loadr R18, 0
	loadr R19, 0
	loadr R20, 0
	; Print System Information
	push BIOS_STRING
	syscall SYSCALL_PUTS
	pop
	push CPU_STRING
	syscall SYSCALL_PUTS
	pop
	; Load Interrupts and jump to 1MB 
	loadi 0x20, get_system_information
	; Run the program
	pushar R0
	ret
get_system_information:
	push MACHINE_VER
	push BIOS_VER
	push IS_UNIX
	ret
CPU_STRING: db "R3X CPU, Based upon FVM Technology. (C) Benderx2", 0x0A, 0
BIOS_STRING: db "R3X BIOS 0.4.0a --- Cheesecake", 0x0A, 0
is_exec: dd 0
_END:
	times 512 - _END db 0
_MEM_AFTR:


	
	
