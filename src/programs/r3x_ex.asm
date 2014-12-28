include 'libR3X/libR3X.inc'
include 'simplelib.inc'
_init:
	loadsimplelib
	simplelib_myfunc1
	loadr R20, 123456
	Console.Write "Hello, World!"
	Console.NewLine
	Console.Write "This is the beta preview of R3X (read : REX) virtual machine"
	Console.NewLine
	Console.Write "Feel free to visit https://www.github.com/Benderx2/R3X for more information!"
	Console.NewLine
	Console.Write "Dispatching a counting job..."
	System.DispatchJob randomjob
	Console.NewLine
	Console.Write "Wait as long as you want... then press a key"
	Console.NewLine
	Console.Input string, 5
	Console.Write "Counted "
	loadr R0, result
	lodsd
	pushr R1
	Console.WriteInt
	pushr R20
	syscall SYSCALL_PUTI
	Console.WaitKey
	System.Quit 0
randomjob:
	loadr R20, 678910
	; Note: This thread has it's own registers, stack, and call stack!
	loadr R0, result
	loadr R1, 0
.loop1:
	incr R1
	stosd
	; A system.quit here will close ONLY this thread, while a system.quit in our init function will close all jobs.
	jmp .loop1;
_end_text:	
_data:	
	 ; Don't forget to include the data section!
	 STORE_SECTION_DATA
	 result: dd 0
	string: times 256 db 0
_end_data:
_bss:
	 STORE_SECTION_BSS
_end_bss:
