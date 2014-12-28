macro loadsimplelib {
	pushr R0
	pushr R1
	pushstring "simplelib.ro"	
	syscall SYSCALL_LOADDYNAMIC
	popr R1
	loadr R0, simplelibid
	stosd
	pop
	popr R1
	popr R0
}
macro simplelib_myfunc1 {
	pushr R0
	pushr R1
	loadr R0, simplelibid
	lodsd
	pushr R1
	push 1
	calldynamic
	pop
	pop
	popr R1
	popr R0
}
simplelibid: dd 0
