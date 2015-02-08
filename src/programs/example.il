
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

	; debug line 2
function x

	; debug line 3
	loadr R1, s0
	call print_s
	call print_n

	; debug line 4
	loadr R8, 5
	loadsr R8
	popr R1
	pushr R0
	pushr R1
	loadr R0, vx.a
	stosd
	popr R1
	popr R0

	; debug line 5
	loadr R1, s1
	call print_s
	loadrm dword, R1, vx.a
	call print_i
	call print_n

	; debug line 6
	loadr R8, 4
	loadsr R8
	popr R1
	loadrr R9, R1
	loadr R1, 15
	loadrr R2, R9
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R0
	pushr R1
	loadr R0, vx.b
	stosd
	popr R1
	popr R0

	; debug line 7
	loadr R1, s2
	call print_s
	loadrm dword, R1, vx.b
	call print_i
	call print_n

	; debug line 8
	loadr R1, 5
	pushr R0
	pushr R1
	loadr R0, vx.f
	stosd
	popr R1
	popr R0

	; debug line 9
	loadr R1, s3
	call print_s
	loadrm dword, R1, vx.f
	call print_i
	call print_n

	; debug line 10
	loadr R8, 3
	loadsr R8
	popr R1
	loadrr R7, R1
	ret

	; debug line 11
endfunction x

	; debug line 12
function add2numbers

	; debug line 13
	loadr R8, 4
	loadsr R8
	popr R1
	loadrr R9, R1
	loadr R8, 3
	loadsr R8
	popr R1
	loadrr R2, R9
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R0
	pushr R1
	loadr R0, vadd2numbers.a
	stosd
	popr R1
	popr R0

	; debug line 14
	loadrm dword, R1, vadd2numbers.a
	loadrr R7, R1
	ret

	; debug line 15
endfunction add2numbers

	; debug line 16
function subtract2numbers

	; debug line 17
	loadr R1, 1
	pushr R1

	loadr R1, 3
	pushr R1

; Save utility registers
	pushr R9
	pushr R10
	call add2numbers
	popr R10
	popr R9
	popn 2
	loadrr R1, R7
	loadr R8, 4
	loadsr R8
	popr R1
	loadrr R9, R1
	loadr R8, 3
	loadsr R8
	popr R1
	loadrr R2, R9
	pushr R2
	pushr R1
	sub
	popr R1
	pop
	pop

	pushr R0
	pushr R1
	loadr R0, vsubtract2numbers.a
	stosd
	popr R1
	popr R0

	; debug line 18
	loadrm dword, R1, vsubtract2numbers.a
	loadrr R7, R1
	ret

	; debug line 19
endfunction subtract2numbers

	; debug line 20
function misc

	; debug line 21
	loadr R1, s4
	call print_s
	loadrm dword, R1, vmisc.f
	call print_i
	call print_n

	; debug line 22

	; debug line 23
	loadr R1, 0
	loadrr R7, R1
	ret

	; debug line 24
endfunction misc

	; debug line 25
function main

	; debug line 26
	loadr R1, 1
	pushr R0
	pushr R1
	loadr R0, vmain.f
	stosd
	popr R1
	popr R0

	; debug line 27
; Save utility registers
	pushr R9
	pushr R10
	call misc
	popr R10
	popr R9
	popn 0
	loadrr R1, R7
	loadr R1, 24
	pushr R0
	pushr R1
	loadr R0, vmain.c
	stosd
	popr R1
	popr R0

	; debug line 28
	loadr R1, 100
	pushr R1

	loadr R1, 200
	pushr R1

	loadrm dword, R1, vmain.c
	loadrr R10, R1
	loadr R1, 5
	loadrr R2, R10
	pushr R1
	pushr R2
	mul
	popr R1
	pop
pop
	
	loadrr R9, R1
	loadr R1, 2
	loadrr R2, R9
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R1

; Save utility registers
	pushr R9
	pushr R10
	call x
	popr R10
	popr R9
	popn 3
	loadrr R1, R7
	pushr R0
	pushr R1
	loadr R0, vmain.retval
	stosd
	popr R1
	popr R0

	; debug line 29
	loadr R1, s5
	call print_s
	loadrm dword, R1, vmain.retval
	call print_i
	call print_n

	; debug line 30
	loadrm dword, R1, vmain.retval
	loadrr R4, R1
	loadrm dword, R1, vmain.c
	loadrr R10, R1
	loadr R1, 5
	loadrr R2, R10
	pushr R1
	pushr R2
	mul
	popr R1
	pop
pop
	
	loadrr R9, R1
	loadr R1, 2
	loadrr R2, R9
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	cmpr R4, R1
	jne i30
	jmp main.lcorrect
i30:

	; debug line 31
	loadr R1, s6
	call print_s
	call print_n

	; debug line 32
	jmp _exit

	; debug line 33
main.lcorrect:
	loadr R1, s7
	call print_s
	call print_n

	; debug line 34
	loadr R1, s3
	call print_s
	loadrm dword, R1, vmain.f
	call print_i
	call print_n

	; debug line 35
	loadr R1, s8
	call print_s
	call print_n

	; debug line 36
	loadr R1, 57
	pushr R1

	loadr R1, 90
	pushr R1

; Save utility registers
	pushr R9
	pushr R10
	call add2numbers
	popr R10
	popr R9
	popn 2
	loadrr R1, R7
	pushr R0
	pushr R1
	loadr R0, vmain.result
	stosd
	popr R1
	popr R0

	; debug line 37
	loadr R1, s9
	call print_s
	loadrm dword, R1, vmain.result
	call print_i
	call print_n

	; debug line 38
	loadr R1, s10
	call print_s
	call print_n

	; debug line 39
	loadr R1, 30
	pushr R1

	loadr R1, 10
	pushr R1

; Save utility registers
	pushr R9
	pushr R10
	call subtract2numbers
	popr R10
	popr R9
	popn 2
	loadrr R1, R7
	pushr R0
	pushr R1
	loadr R0, vmain.result
	stosd
	popr R1
	popr R0

	; debug line 40
	loadr R1, s9
	call print_s
	loadrm dword, R1, vmain.result
	call print_i
	call print_n

	; debug line 41
	jmp _exit

	; debug line 42
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

	vx.a: rd 1
	vx.b: rd 1
	vx.f: rd 1
	vadd2numbers.a: rd 1
	vsubtract2numbers.a: rd 1
	vmisc.f: rd 1
	vmain.f: rd 1
	vmain.c: rd 1
	vmain.retval: rd 1
	vmain.result: rd 1
}
.data {

	s0: db "I'm a function take takes 3 args", 0
	s1: db "the number got in the first arg is: ", 0
	s2: db "the number got in the second arg + 15 is :", 0
	s3: db "f in this scope is: ", 0
	s4: db "f in this scope is", 0
	s5: db "return value: ", 0
	s6: db "lol, this failed.", 0
	s7: db "lol it's right", 0
	s8: db "adding 2 numbers: 57 and 90", 0
	s9: db "result = ", 0
	s10: db "subtracting 2 numbers: 30 and 10", 0
}
end
; Task Completed -- Assemble with FASM 
