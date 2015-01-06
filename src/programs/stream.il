include 'libR3X/libR3X.pkg'
.text {
	pushstring "./myfile.txt"
	syscall SYSCALL_OPENSTREAM
	push mybuf
	push 255
	syscall SYSCALL_READSTREAM
	pop
	pop
	pop
	syscall SYSCALL_CLOSESTREAM
	Console.Write "Read 255 bytes of ./myfile.txt: (Press a key to quit)"
	Console.NewLine
	Console.WritePointer mybuf
	Console.WaitKey
	exit
}
.data {
	mybuf: times 256 db 0
}
end
