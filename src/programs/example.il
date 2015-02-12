
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

	; debug line 3
function x

	; debug line 4
	pushar R9
	pushar R9
	loadr R1, 1075419546
	loadrr R9, R1
	pushar R9
	loadr R1, 1080033280
	loadrr R9, R1
	loadr R1, 1083388723
	pushr R9
	pushr R1
	fmul
	popr R1
	popn 2
	popar R9

	pushr R9
	pushr R1
	fadd
	popr R1
	popn 2
	popar R9

	loadrr R9, R1
	loadr R1, 1085485875
	pushr R9
	pushr R1
	fadd
	popr R1
	popn 2
	popar R9

	pushr R0
	pushr R1
	loadr R0, vx.i
	stosd
	popr R1
	popr R0

	; debug line 5
	loadr R1, s0
	call print_s
	loadrm dword, R1, vx.i
	call print_f
	call print_n

	; debug line 6
	loadr R1, s1
	call print_s
	call print_n

	; debug line 7
	loadr R8, 7
	loadsr R8
	popr R1
	pushr R0
	pushr R1
	loadr R0, vx.a
	stosd
	popr R1
	popr R0

	; debug line 8
	loadr R1, s2
	call print_s
	loadrm dword, R1, vx.a
	call print_i
	call print_n

	; debug line 9
	loadr R8, 6
	loadsr R8
	popr R1
	pushar R1
	loadr R1, 15
	popar R2
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

	; debug line 10
	loadr R1, s3
	call print_s
	loadrm dword, R1, vx.b
	call print_i
	call print_n

	; debug line 11
	loadr R1, 5
	pushr R0
	pushr R1
	loadr R0, vx.f
	stosd
	popr R1
	popr R0

	; debug line 12
	loadr R1, s4
	call print_s
	loadrm dword, R1, vx.f
	call print_i
	call print_n

	; debug line 13
	loadr R8, 5
	loadsr R8
	popr R1
	loadrr R7, R1
	ret

	; debug line 14
endfunction x

	; debug line 15
function add2numbers

	; debug line 16
	loadr R8, 6
	loadsr R8
	popr R1
	pushar R1
	loadr R8, 5
	loadsr R8
	popr R1
	popar R2
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

	; debug line 17
	loadrm dword, R1, vadd2numbers.a
	loadrr R7, R1
	ret

	; debug line 18
endfunction add2numbers

	; debug line 19
function subtract2numbers

	; debug line 20
	loadr R8, 6
	loadsr R8
	popr R1
	pushar R1
	loadr R8, 5
	loadsr R8
	popr R1
	popar R2
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

	; debug line 21
	loadrm dword, R1, vsubtract2numbers.a
	loadrr R7, R1
	ret

	; debug line 22
endfunction subtract2numbers

	; debug line 23
function misc

	; debug line 24
	loadr R1, s5
	call print_s
	loadrm dword, R1, vmisc.f
	call print_i
	call print_n

	; debug line 25

	; debug line 26
	loadr R1, 0
	loadrr R7, R1
	ret

	; debug line 27
endfunction misc

	; debug line 28
function main

	; debug line 29
	loadr R1, s6
	call print_s
	loadrm dword, R1, vmain.lol
	call print_i
	call print_n

	; debug line 30
	loadr R1, 0
	pushr R0
	pushr R1
	loadr R0, vmain.xxx
	stosd
	popr R1
	popr R0

	; debug line 31
	loadr R1, 0
	pushr R0
	pushr R1
	loadr R0, vmain.y_z
	stosd
	popr R1
	popr R0

	; debug line 32
	loadr R1, 1
	pushr R0
	pushr R1
	loadr R0, vmain.f
	stosd
	popr R1
	popr R0

	; debug line 33
; Save utility registers
	pushr R4
	pushr R5
	pushr R9
	pushr R10
	call misc
	popr R10
	popr R9
	popr R5
	popr R4
	popn 0
	loadrr R1, R7
	loadr R1, 24
	pushr R0
	pushr R1
	loadr R0, vmain.c
	stosd
	popr R1
	popr R0

	; debug line 34
	loadr R1, 100
	pushr R1

	loadr R1, 200
	pushr R1

	loadrm dword, R1, vmain.c
	pushar R1
	loadr R1, 5
	popar R2
	pushr R1
	pushr R2
	mul
	popr R1
	pop
pop
	
	pushar R1
	loadr R1, 2
	popar R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R1

; Save utility registers
	pushr R4
	pushr R5
	pushr R9
	pushr R10
	call x
	popr R10
	popr R9
	popr R5
	popr R4
	popn 3
	loadrr R1, R7
	pushr R0
	pushr R1
	loadr R0, vmain.retval
	stosd
	popr R1
	popr R0

	; debug line 35
	loadr R1, s7
	call print_s
	loadrm dword, R1, vmain.retval
	call print_i
	call print_n

	; debug line 36
	loadrm dword, R1, vmain.retval
	loadrr R4, R1
	loadrm dword, R1, vmain.c
	pushar R1
	loadr R1, 5
	popar R2
	pushr R1
	pushr R2
	mul
	popr R1
	pop
pop
	
	pushar R1
	loadr R1, 2
	popar R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	cmpr R4, R1
	jne i36
	jmp main.lcorrect
i36:

	; debug line 37
	loadr R1, s8
	call print_s
	call print_n

	; debug line 38
	jmp _exit

	; debug line 39
main.lcorrect:
	loadr R1, s9
	call print_s
	call print_n

	; debug line 40
	loadr R1, s4
	call print_s
	loadrm dword, R1, vmain.f
	call print_i
	call print_n

	; debug line 41
	loadr R1, s10
	call print_s
	call print_n

	; debug line 42
	loadr R1, 57
	pushr R1

	loadr R1, 90
	pushr R1

; Save utility registers
	pushr R4
	pushr R5
	pushr R9
	pushr R10
	call add2numbers
	popr R10
	popr R9
	popr R5
	popr R4
	popn 2
	loadrr R1, R7
	pushr R0
	pushr R1
	loadr R0, vmain.result
	stosd
	popr R1
	popr R0

	; debug line 43
	loadr R1, s11
	call print_s
	loadrm dword, R1, vmain.result
	call print_i
	call print_n

	; debug line 44
	loadr R1, s12
	call print_s
	call print_n

	; debug line 45
	loadr R1, 30
	pushr R1

	loadr R1, 10
	pushr R1

; Save utility registers
	pushr R4
	pushr R5
	pushr R9
	pushr R10
	call subtract2numbers
	popr R10
	popr R9
	popr R5
	popr R4
	popn 2
	loadrr R1, R7
	pushr R0
	pushr R1
	loadr R0, vmain.result
	stosd
	popr R1
	popr R0

	; debug line 46
	loadr R1, s11
	call print_s
	loadrm dword, R1, vmain.result
	call print_i
	call print_n

	; debug line 47
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

	vx.i: rd 1
	vx.a: rd 1
	vx.b: rd 1
	vx.f: rd 1
	vadd2numbers.a: rd 1
	vsubtract2numbers.a: rd 1
	vmisc.f: rd 1
	vmain.lol: rd 1
	vmain.xxx: rd 1
	vmain.y_z: rd 1
	vmain.f: rd 1
	vmain.c: rd 1
	vmain.retval: rd 1
	vmain.result: rd 1
}
.data {

	s0: db "float i = ", 0
	s1: db "I'm a function take takes 3 args", 0
	s2: db "the number got in the first arg is: ", 0
	s3: db "the number got in the second arg + 15 is :", 0
	s4: db "f in this scope is: ", 0
	s5: db "f in this scope is", 0
	s6: db "result of strlen = ", 0
	s7: db "return value: ", 0
	s8: db "lol, this failed.", 0
	s9: db "lol it's right", 0
	s10: db "adding 2 numbers: 57 and 90", 0
	s11: db "result = ", 0
	s12: db "subtracting 2 numbers: 30 and 10", 0
}
end
; Task Completed -- Assemble with FASM 
