
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

	; debug line 4
	loadr R1, 256
	pushr R0
	pushr R1
	call alloc_n
	loadr R0, vmain.buffer
	stosd
	popr R1
	popr R0

	; debug line 5
	loadrm dword, R1, vmain.buffer
	loadrr R5, R1
	loadr R1, 65348
	pushr R0
	loadrr R0, R5
	stosw
	popr R0

	; debug line 6
	loadrm dword, R1, vmain.buffer
	pushr R0
	loadrr R0, R1
	lodsb
	popr R0
	loadrr R4, R1
	loadr R1, 68
	cmpr R4, R1
	jne i6
	jmp main.lyes
i6:

	; debug line 7
	loadr R1, s1
	call print_s
	call print_n

	; debug line 8
	jmp _exit

	; debug line 9
main.lyes:
	loadr R1, s2
	call print_s
	call print_n

	; debug line 10
	jmp _exit

	; debug line 11
endfunction main

; exit to operating system

_exit:
	Console.WaitKey
	System.Quit 0

; print_i excluded

; print string to terminal
print_s:
	pushr R1
	syscall SYSCALL_PUTS
	popr R1
	ret

; print_t excluded

; print a newline to the terminal
print_n:
	push 0x0A
	syscall SYSCALL_PUTCH
	pop
	ret

; input_i excluded
; Allocate n bytes of memory
alloc_n:
	pushr R1
	syscall SYSCALL_ALLOC
	popr R1
	pop
	ret
; Output an IEEE-754 floating point number to stdout
print_f:
	pushr R1
	syscall SYSCALL_PUTF
	popr R1
	ret
; Free an allocated region
free:
	pushr R1
	syscall SYSCALL_FREE
	pop
	ret

}
.bss {

	vmain.buffer: rd 1
}
.data {

	s0: db "If stuff", 0
	s1: db "Uh, error!", 0
	s2: db "Yay, little endian!", 0
}
end
; Task Completed -- Assemble with FASM 
