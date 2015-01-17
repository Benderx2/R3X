include 'libR3X/libR3X.pkg'
.text {
	function main
		pushstring "Hello, World!"
		syscall SYSCALL_PUTS
		push 0
		exit
	endfunction main
}
end
