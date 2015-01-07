include 'libR3X/libR3X.pkg'
.text {
	function main, "main"
	_loop:
	push 0
	jmp _loop
	endfunction main
}
end
