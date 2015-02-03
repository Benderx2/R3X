include 'libR3X/libR3X.pkg'
.text {
function main



	; debug line 1
	loadr R1, s0
	call print_s
	call print_n

	; debug line 2
	jmp lmylabel

	; debug line 3
	loadr R1, s1
	call print_s
	call print_n

	; debug line 4
lmylabel:

	; debug line 5
	loadr R1, 20
	pushr R0
	pushr R1
	loadr R0, vx
	stosd
	popr R1
	popr R0

	; debug line 6
	loadr R1, 5
	pushr R0
	pushr R1
	loadr R0, vy
	stosd
	popr R1
	popr R0

	; debug line 7
	loadrm dword, R1, vx
	pushr R1
	loadrm dword, R1, vy
	popr R2
	loadr R4, 0
	pushr R2
	pushr R1
	div
	popr R1
	pop
	pop
	pushr R0
	pushr R1
	loadr R0, vz
	stosd
	popr R1
	popr R0

	; debug line 8
	loadr R1, s2
	call print_s
	loadrm dword, R1, vz
	call print_i
	call print_n

	; debug line 9
	loadr R1, 50
	pushr R1
	loadr R1, 2
	popr R2
	loadr R4, 0
	pushr R2
	pushr R1
	and
	popr R1
	pop
	pop
	pushr R0
	pushr R1
	loadr R0, va
	stosd
	popr R1
	popr R0

	; debug line 10
	loadr R1, s3
	call print_s
	loadrm dword, R1, va
	call print_i
	call print_n

	; debug line 11
	call input_i
	pushr R0
	pushr R1
	loadr R0, vy
	stosd
	popr R1
	popr R0

	; debug line 12
	loadr R1, s4
	call print_s
	loadrm dword, R1, vy
	call print_i
	call print_n

	; debug line 13
	call input_i
	pushr R0
	pushr R1
	loadr R0, vy
	stosd
	popr R1
	popr R0

	; debug line 14
	push 0

	; debug line 15
	exit
endfunction main

	; exit to operating system
	; program falls through to here when there is no explicit end
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

	; read a number from the terminal
	input_i:
	syscall SYSCALL_GETC
	push 0
	cmp
	pop
	popr R1
	je input_i
	ret

}
.bss {

	va: rd 1
	vx: rd 1
	vy: rd 1
	vz: rd 1
}
.data {

	s0: db "Testing Division and bitwise", 0
	s1: db "Testing division", 0
	s2: db "Div: ", 0
	s3: db "Or: ", 0
	s4: db "Y = ", 0
}
end
; Task Completed -- Assemble with FASM
