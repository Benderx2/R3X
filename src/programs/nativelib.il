include 'libR3X/libR3X.pkg'
.text {
	function main
		pushstring "./mylib.so"
		loadlib
		Console.Write "Loaded mylib.so"
		Console.NewLine
		pushstring "Hello World!"
		push 0
		push 0
		push 0
		push 0
		pushstring "./mylib.so"
		pushstring "myfunc"
		libexec
		Console.Write "Function returned: "
		syscall SYSCALL_PUTI
		Console.WaitKey
		exit
	endfunction main
}
end
