
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
function main



	; debug line 1
	loadr R1, s0
	call print_s
	call print_n

	; debug line 2
	jmp lmylabel
	; debug line 3
lmylabel:

	; debug line 4

	; debug line 5
	loadr R1, 4
	pushr R0
	pushr R1
	call alloc_n
	loadr R0, vbuffer
	stosd
	popr R1
	popr R0

	; debug line 6
	loadr R1, 100
	pushr R0
	pushr R1
	loadr R0, vB
	stosd
	popr R1
	popr R0

	; debug line 7

	; debug line 8

	; debug line 9
	loadrm dword, R1, vbuffer
	loadrr R5, R1
	loadr R1, 168882893
	pushr R1
	loadr R1, 1051324
	popr R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R1
	loadr R1, 697
	pushr R1
	loadr R1, 963
	popr R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R1
	loadr R1, 1
	popr R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	popr R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R0
	loadrr R0, R5
	stosd
	popr R0


	; debug line 10
	loadrm dword, R1, vbuffer
	pushr R0
	loadrr R0, R1
	lodsw
	popr R0

	pushr R0
	pushr R1
	loadr R0, vX
	stosd
	popr R1
	popr R0

	; debug line 11
	loadr R1, s1
	call print_s
	loadrm dword, R1, vX
	call print_i
	call print_t
	loadrm dword, R1, vbuffer
	pushr R0
	loadrr R0, R1
	lodsd
	popr R0

	call print_i
	call print_n

	; debug line 12
	loadr R1, 0
	pushr R0
	pushr R1
	loadr R0, vS
	stosd
	popr R1
	popr R0

	; debug line 13
	jmp _exit
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
; Free an allocated region
free:
	pushr R1
	syscall SYSCALL_FREE
	pop
	ret

}
.bss {

	vbuffer: rd 1
	vB: rd 1
	vX: rd 1
	vS: rd 1
}
.data {

	s0: db "Array stuff", 0
	s1: db "(uint8_t)S[0] and (uint32_t)S[0] = ", 0
}
end
; Task Completed -- Assemble with FASM 
