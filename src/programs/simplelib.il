include 'libR3X/dynR3X.pkg'
.text {
function test_func
	; For dynamically loaded libraries, DONT USE PUSH WHILE REFERRING TO ADDRESSES, use pushad (push relative address)
	pushad str01
	syscall SYSCALL_PUTS
	pop
	loadrm word, R1, word_data
	pushr R1
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
