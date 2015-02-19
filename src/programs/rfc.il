include 'libR3X/libR3X.pkg'
.text {
	function main
		loadr64 R0, 0xFFFFFFFFFFFFFFFE
		addr R0, 1
		pushr R0
		syscall SYSCALL_PUTI
		pop
		Console.WaitKey
		exit
	endfunction main
}
end
