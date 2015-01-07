include 'libR3X/libR3X.pkg'
.text {
	function main
	; This example shows exception handling
	push _invalidexception_handler
	push INVALID_ACCESS
	catch
	popn 2
	push _invalid_opcode_handler
	push INVALID_OPCODE
	catch 
	popn 2
	push _exception_handler
	push R_EXCEP
	catch 
	popn 2
	; Do an invalid access!
	loadr R0, 0xFFFFFFF
	lodsw ; BOOOOOOOOOOOOOOOOOOOOOOM
_doinvalid_opcode_exception:
	db 0xFF
_dorandom_exception:
	push 15 ; Push random error code
	push R_EXCEP
	throw
	exit ; This will never be called lol
_invalidexception_handler:
	push INVAEXCEPTION_STRING
	syscall SYSCALL_PUTS
	pop
	handle
	jmp _doinvalid_opcode_exception
_invalid_opcode_handler:
	push INVOEXCEPTION_STRING
	syscall SYSCALL_PUTS
	pop
	handle
	jmp _dorandom_exception
_exception_handler:
	pop ; Pop out exception code
	push GLOBAL_EXCEPTION_STRING
	syscall SYSCALL_PUTS
	pop
	; The program pushed the error code remember?
	syscall SYSCALL_PUTI
	Console.WaitKey
	handle
	System.Quit 0
	endfunction main
}
.data {
	INVAEXCEPTION_STRING: db 'Invalid Memory Access. Exception Detected! Causing another one.. ;).', 0x0A, 0
	INVOEXCEPTION_STRING: db 'Invalid Opcode! Causing Another one... lol', 0x0A, 0
	GLOBAL_EXCEPTION_STRING: db "Well, the program threw an exception on it's own.", 0x0A, "So well, fuck it. I'm closing", 0x0A, "And here's the error code: ", 0
}
end
