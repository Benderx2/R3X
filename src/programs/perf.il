; Simple performance test
include 'libR3X/libR3X.pkg'
.text {
	function main
	syscall SYSCALL_GETCLOCK
	popr R1
	loadr R0, save_float
	stosd
	; Load R7 with 1
	loadr R7, 1
.do_perf_test:
	; Push R7
	pushr R7
	fsqrt
	; Increment R7
	incr R7
	; Is R7 == 10000000
	pushr R7
	push 10000000
	cmp
	popn 4
	je .done
	jmp .do_perf_test
.done:
	syscall SYSCALL_GETCLOCK
	popr R1
	pushr R1
	syscall SYSCALL_GETCLOCKSPERSEC
	fdiv
	popr R1
	popn 2
	push desc
	syscall SYSCALL_PUTS
	pushr R1
	syscall SYSCALL_PUTF
	Console.WaitKey
	push 0
	exit
	endfunction main
}
.data {
	save_float: dd 0
	desc: db 'Found the square root of numbers 1-10000000 in (seconds) : ', 0
}
end
