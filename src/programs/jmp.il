include 'libR3X/libR3X.pkg'
.text {
	function main
		Console.Write "Current IP: "
		jmp current_ip
current_ip:
		; Address of "current_ip" is pushed to stack
		puship
		syscall SYSCALL_PUTI
		pop
		; Jump 269 bytes after "rel_jump". NOTE: This jump is position independent.
		calll rel_jump
		System.Quit 0
		times 269 db 0x1F ; Exit opcode.
finally_exit: 
		Console.Write "Exitting!"
		Console.WaitKey
		exit
rel_jump:
		Console.Write "Relative Jump!"
		jmpl finally_exit
	endfunction main
}
end
