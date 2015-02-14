include 'libR3X/dynR3X.pkg'
; NOTE: CODE MUST BE RELOCATABLE!
; USE RELOCATABLE BRANCH INSTRUCTIONS!
; jmpl, jzl, jll, jel, jgl, and calll. (an 'l' suffix)
; Also, add the value of R20 (which is guarenteed to be the load address of the library)
; before addressing local addresses.
.text {
function test_func
	; Dynamically loaded libraries must 
	; be relocatable that is, position independent.
	loadr R1, str01
	; Load address of library is in R20
	addrr R1, R20
	pushr R1
	syscall SYSCALL_PUTS
	pop
	loadr R0, word_data
	addrr R0, R20
	lodsw
	pushr R1
	syscall SYSCALL_PUTI
	pop
	push 0xA
	syscall SYSCALL_PUTCH
	pop
	ret
endfunction test_func
}
.data {
	str01: db 'Hello from dynamic library! I can read stuff from addresses! word_data=',  0
	word_data: dw 212
	dd 0xFFF3FFFF
}
end
