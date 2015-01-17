include 'libR3X/libR3X.pkg'
.text {
	function main
		Console.Write "Current IP: "
current_ip:
		; Address of "current_ip" is pushed to stack
		puship
		syscall SYSCALL_PUTI
		pop
		; Jump 256 bytes after "rel_jump"
		jmpl 256
rel_jump:
		times 256 db 0x1F ; Exit opcode
		Console.Write "Relative Jump!"
		Console.WaitKey
		System.Quit 0
	endfunction main
}
end
