
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
function factorial

	; debug line 2
	loadr R8, 3
	loadsr R8
	popr R1
	loadrr R4, R1
	loadr R1, 1
	cmpr R4, R1
	jg i2
	jmp factorial.lfunc_return
i2:

	; debug line 3
	loadr R8, 3
	loadsr R8
	popr R1
	loadrr R10, R1
	loadr R8, 3
	loadsr R8
	popr R1
	loadrr R9, R1
	loadr R1, 1
	loadrr R2, R9
	pushr R2
	pushr R1
	sub
	popr R1
	pop
	pop

	pushr R1

; Save utility registers
	pushr R9
	pushr R10
	call factorial
	popr R10
	popr R9
	popn 1
	loadrr R1, R7
	loadrr R2, R10
	pushr R1
	pushr R2
	mul
	popr R1
	pop
pop
	
	loadrr R7, R1
	ret

	; debug line 4
factorial.lfunc_return:
	loadr R1, 1
	loadrr R7, R1
	ret

	; debug line 5
endfunction factorial

	; debug line 7
function main

	; debug line 8
	loadr R1, 9
	pushr R1

; Save utility registers
	pushr R9
	pushr R10
	call factorial
	popr R10
	popr R9
	popn 1
	loadrr R1, R7
	pushr R0
	pushr R1
	loadr R0, vmain.fact
	stosd
	popr R1
	popr R0

	; debug line 9
	loadr R1, s0
	call print_s
	loadrm dword, R1, vmain.fact
	call print_i
	call print_n

	; debug line 10
	jmp _exit

	; debug line 11
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
; Free an allocated region
free:
	pushr R1
	syscall SYSCALL_FREE
	pop
	ret

}
.bss {

	vmain.fact: rd 1
}
.data {

	s0: db "factorial of 9: ", 0
}
end
; Task Completed -- Assemble with FASM 
