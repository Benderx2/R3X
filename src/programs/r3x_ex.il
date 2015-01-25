include 'libR3X/libR3X.pkg'
import 'libR3X/corelib.pkg'
.text {
	function main
	Console.Write "Allocating Page, page address: "
	System.AllocatePage
	Console.WriteInt
	Console.NewLine
	Console.Write "Calling Dynamic Library (simplelib.ro)"
	Console.NewLine
	pushr R0
	pushr R1
	; Load lib.
	call loadsimplelib
	; run lib
	call callsimplelib
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
	Console.WaitKey
	Console.Write "Counted "
	loadr R0, result
	lodsd
	pushr R1
	Console.WriteInt
	Console.WaitKey
	System.Quit 0
	endfunction main
	function randomjob
	loadr R20, 678910
	; Note: This thread has it's own registers, stack, and call stack!
	loadr R0, result
	loadr R1, 0
.loop1:
	incr R1
	stosd
	; A system.quit here will close ONLY this thread, while a system.quit in our init function will close all jobs.
	jmp .loop1;
	endfunction randomjob
	
	function loadsimplelib
		push libname
		syscall SYSCALL_LOADDYNAMIC
		popr R1
		loadr R0, simplelibid
		stosd
		pop
		popr R1
		popr R0
		ret
	endfunction loadsimplelib
	
	function callsimplelib
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
		ret
	endfunction callsimplelib
}	
.data {
	libname: db "simplelib.ro", 0
}
.bss {
	string: times 256 rb 0
	simplelibid: rd 0
	result: rd 0
}
end
