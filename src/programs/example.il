
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

	; debug line 3
	loadr R1, 256
	pushr R0
	pushr R1
	call alloc_n
	loadr R0, vbuffer
	stosd
	popr R1
	popr R0

	; debug line 4
	loadrm dword, R1, vbuffer
	loadrr R5, R1
	loadr R1, 65348
	pushr R0
	loadrr R0, R5
	stosw
	popr R0


	; debug line 5
	loadrm dword, R1, vbuffer
	pushr R0
	loadrr R0, R1
	lodsb
	popr R0

	loadrr R4, R1
	loadr R1, 68
	cmpr R4, R1
	jne i5
	jmp lyes
i5:

	; debug line 6
	loadr R1, s1
	call print_s
	call print_n

	; debug line 7
	jmp _exit

	; debug line 8
lyes:

	; debug line 9
	loadr R1, s2
	call print_s
	call print_n

	; debug line 10
	jmp _exit
endfunction main

	; exit to operating system
	; program falls through to here when there is no explicit end
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
; Free an allocated region
free:
	pushr R1
	syscall SYSCALL_FREE
	pop
	ret

}
.bss {

	vbuffer: rd 1
}
.data {

	s0: db "If stuff", 0
	s1: db "Uh, error!", 0
	s2: db "Yay, little endian!", 0
}
end
; Task Completed -- Assemble with FASM 
