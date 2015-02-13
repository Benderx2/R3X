
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
	jmp main.lmylabel

	; debug line 4
	loadr R1, s1
	call print_s
	call print_n

	; debug line 5
main.lmylabel:
	loadr R1, 21
	pushr R0
	pushr R1
	loadr R0, vmain.X
	stosd
	popr R1
	popr R0

	; debug line 6
	loadr R1, 5
	pushr R0
	pushr R1
	loadr R0, vmain.Y
	stosd
	popr R1
	popr R0

	; debug line 7
	loadrm dword, R1, vmain.X
	pushar R1
	loadrm dword, R1, vmain.Y
	popar R2
	pushr R2
	pushr R1
	div
	popr R1
	pop
	pop
	pushr R0
	pushr R1
	loadr R0, vmain.Z
	stosd
	popr R1
	popr R0

	; debug line 8
	loadrm dword, R1, vmain.X
	pushar R1
	loadrm dword, R1, vmain.Y
	popar R2
pushr R2
	pushr R1
	mod
	popr R1
	popn 2
	pushr R0
	pushr R1
	loadr R0, vmain.R
	stosd
	popr R1
	popr R0

	; debug line 9
	loadr R1, s2
	call print_s
	loadrm dword, R1, vmain.R
	call print_i
	call print_n

	; debug line 10
	loadr R1, s3
	call print_s
	loadrm dword, R1, vmain.Z
	call print_i
	call print_n

	; debug line 11
	loadr R1, 50
	pushar R1
	loadr R1, 2
	popar R2
	pushr R2
	pushr R1
	and
	popr R1
	pop
	pop
	pushr R0
	pushr R1
	loadr R0, vmain.A
	stosd
	popr R1
	popr R0

	; debug line 12
	loadr R1, 50
	pushar R1
	loadr R1, 2
	popar R2
pushr R2
	pushr R1
	shl
	popr R1
	popn 2
	pushr R0
	pushr R1
	loadr R0, vmain.Q
	stosd
	popr R1
	popr R0

	; debug line 13

	; debug line 14
	loadr R1, 1
	pushar R1
	loadrm dword, R1, vmain.Q
	popar R2
pushr R1
	not
	popr R1
	pop
	pushr R0
	pushr R1
	loadr R0, vmain.C
	stosd
	popr R1
	popr R0

	; debug line 15
	loadr R1, s4
	call print_s
	loadrm dword, R1, vmain.A
	call print_i
	call print_n

	; debug line 16
	loadr R1, s5
	call print_s
	loadrm dword, R1, vmain.Q
	call print_i
	call print_t
	loadrm dword, R1, vmain.C
	call print_i
	call print_n

	; debug line 17
	call input_i
	pushr R0
	pushr R1
	loadr R0, vmain.Y
	stosd
	popr R1
	popr R0

	; debug line 18
	loadr R1, s6
	call print_s
	loadrm dword, R1, vmain.Y
	call print_i
	call print_n

	; debug line 19
	loadrm dword, R1, vmain.Y
	call print_i
	call print_n

	; debug line 20
	push 0

	; debug line 21
	exit

	; debug line 22
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

	; read a number from the terminal
input_i:
	syscall SYSCALL_GETC
	push 0
	cmp
	pop
	popr R1
	je input_i
	ret
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

	vmain.X: rd 1
	vmain.Y: rd 1
	vmain.Z: rd 1
	vmain.R: rd 1
	vmain.A: rd 1
	vmain.Q: rd 1
	vmain.C: rd 1
}
.data {

	s0: db "Testing Division and bitwise", 0
	s1: db "Testing division", 0
	s2: db "Mod: ", 0
	s3: db "Div: ", 0
	s4: db "Or: ", 0
	s5: db "Q, C = ", 0
	s6: db "Y = ", 0
}
end
; Task Completed -- Assemble with FASM 
