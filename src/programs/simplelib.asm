include 'libR3X/dynR3X.inc'
_text:
myfunc1:
	; For dynamically loaded libraries, DONT USE PUSH WHILE REFERRING TO ADDRESSES, use pushad (push relative address)
	pushad str01
	syscall SYSCALL_PUTS
	pop
	ret
_end_text:
_export:
	EXPORT_FUNCTION 0x0, 0
	; Export to index 0x1
	EXPORT_FUNCTION 0x1, myfunc1
_end_export:
_data:
	str01: db 'Hello from dynamic library!', 0xA, 0
_end_data:
_bss:
	; BSS here.
_end_bss:
