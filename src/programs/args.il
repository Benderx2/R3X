include 'libR3X/libR3X.pkg'
.text {
	function main
		Console.Write 'Run with: rxvm -exe ./args.exe -args "myarguments"'
		Console.NewLine
		Console.Write "Arugments Passed: "
		syscall SYSCALL_GETARGS
		syscall SYSCALL_PUTS
		Console.WaitKey
		System.Quit 0
	endfunction main
}
end
