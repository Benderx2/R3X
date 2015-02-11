
include 'libR3X/libR3X.pkg'
.text {
	dd 0x56081124
	dd 0x12345678
	dd 0x12335850
	dd 0xFFFF3FFF
	dd 0x23FF0FFF
	dd 0x13370000
	dd 0x66600000
	dd 0xEF7E0016
}

.text {


	; debug line 1
function main

	; debug line 2
	loadr R1, s0
	call print_s
	call print_n

	; debug line 3
	loadr R1, 30
	pushr R0
	pushr R1
	loadr R0, vmain.A
	stosd
	popr R1
	popr R0
