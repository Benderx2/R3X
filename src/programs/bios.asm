; R3X System BIOS
; Configures initial system information
include 'libR3X/r3x_asm.inc'
define MACHINE_VER 10
define BIOS_VER 1
define IS_UNIX 1
init:
	; Print System Information
	push BIOS_STRING
	syscall SYSCALL_PUTS
	pop
	push CPU_STRING
	syscall SYSCALL_PUTS
	pop
	; Load Interrupts and jump to 1MB 
	loadi 0x20, get_system_information
	; Return back to the CPU...
	exit
get_system_information:
	push MACHINE_VER
	push BIOS_VER
	push IS_UNIX
	ret
CPU_STRING: db "REX CPU, Based upon FVM Technology. (C) Benderx2", 0x0A, 0
BIOS_STRING: db "R3X BIOS 0.0.1 --- Beefy Bacon for Linux64", 0x0A, 0
_END:
	times 512 - _END db 0

	
	