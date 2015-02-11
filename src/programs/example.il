
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
	pushr R9
	loadr R1, 1094189056
	loadrr R9, R1
	pushr R9
	loadr R1, 1106544951
	loadrr R9, R1
	loadr R1, 1102582907
	pushr R9
	pushr R1
	fadd
	popr R1
	popn 2
popr R9

	pushr R9
	pushr R1
	fmul
	popr R1
	popn 2
popr R9

	pushr R0
	pushr R1
	loadr R0, vmain.A
	stosd
	popr R1
	popr R0

	; debug line 4
	loadrm dword, R1, vmain.A
	pushr R1
	iconv
	popr R1
	pop
	pushr R0
	pushr R1
	loadr R0, vmain.rounded_a
	stosd
	popr R1
	popr R0

	; debug line 5
	loadr R1, s1
	call print_s
	loadrm dword, R1, vmain.A
	call print_f
	call print_t
	loadrm dword, R1, vmain.rounded_a
	call print_i
	call print_n

	; debug line 6
	jmp _exit

	; debug line 7
endfunction main

; exit to operating system

_exit:
	Console.WaitKey
	System.Quit 0

; print an integer to the terminal
print_i:
	pushr R1
	syscall SYSCALL_PUTI
	popr R1
	ret

; print string to terminal
print_s:
	pushr R1
	syscall SYSCALL_PUTS
	popr R1
	ret

; print a tab to the terminal
print_t:
	push 0x09
	syscall SYSCALL_PUTCH
	pop
	ret

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

	vmain.A: rd 1
	vmain.rounded_a: rd 1
}
.data {

	s0: db "floating point test", 0
	s1: db "result of adding 30.567, 23.01 and the multiplying with 11.5 and rounding it off as int32 ", 0
}
end
; Task Completed -- Assemble with FASM 
