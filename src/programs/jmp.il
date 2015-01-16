include 'libR3X/libR3X.pkg'
.text {
	function main
		Console.Write "Current IP: "
		puship
		syscall SYSCALL_PUTI
		pop
		jmpl 256
		times 256 db 0
		Console.Write "Relative Jump!"
		Console.WaitKey
		System.Quit 0
	endfunction main
}
end
