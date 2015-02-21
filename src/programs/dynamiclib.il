include 'libR3X/libR3X.pkg'
extern test_func, "simplelib.ro"

.text {
	function main
		externcall test_func
		Console.WaitKey
		exit
	endfunction main
}
end
