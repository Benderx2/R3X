include 'libR3X/libR3X.pkg'
.text {
function main



	; debug line 1

	; debug line 2

	; debug line 4

	; debug line 5
l10:
	loadr R1, 0
	pushr R0
	pushr R1
	loadr R0, vi
	stosd
	popr R1
	popr R0

	; debug line 6
l20:
	loadr R1, 1
	pushr R0
	pushr R1
	loadr R0, vx
	stosd
	popr R1
	popr R0

	; debug line 7
l30:
	loadr R1, 0
	pushr R0
	pushr R1
	loadr R0, vy
	stosd
	popr R1
	popr R0

	; debug line 9

	; debug line 10
l40:
	loadr R1, s0
	call print_s
	loadrm dword, R1, vi
	call print_i
	call print_t
	loadrm dword, R1, vx
	call print_i
	call print_n

	; debug line 12

	; debug line 13

	; debug line 14
l50:
	loadrm dword, R1, vx
	pushr R0
	pushr R1
	loadr R0, vz
	stosd
	popr R1
	popr R0

	; debug line 15
l60:
	loadrm dword, R1, vx
	pushr R1
	loadrm dword, R1, vy
	popr R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R0
	pushr R1
	loadr R0, vx
	stosd
	popr R1
	popr R0

	; debug line 16
l70:
	loadrm dword, R1, vz
	pushr R0
	pushr R1
	loadr R0, vy
	stosd
	popr R1
	popr R0

	; debug line 18

	; debug line 19
l80:
	loadrm dword, R1, vi
	pushr R1
	loadr R1, 1
	popr R2
	pushr R1
	pushr R2
	add
	popr R1
	pop
	pop

	pushr R0
	pushr R1
	loadr R0, vi
	stosd
	popr R1
	popr R0

	; debug line 20
l90:
	loadrm dword, R1, vi
	loadrr R4, R1
	loadr R1, 10
	cmpr R4, R1
	jg i20
	jmp l40
i20:
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

}
.bss {

	vi: rd 1
	vx: rd 1
	vy: rd 1
	vz: rd 1
}
.data {

	s0: db "FIB: ", 0
}
end
