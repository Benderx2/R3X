include 'libR3X/libR3X.inc'
.text {	
	Console.Write "Hello, this example will showcase R3X's math functions"
	Console.NewLine
	Console.Write "Sine, Cosine, and tangent of 30 DEG: "
	push 30.0f
	rconv
	fsin
	syscall SYSCALL_PUTF
	pop
	fcos 
	syscall SYSCALL_PUTF
	pop
	ftan 
	syscall SYSCALL_PUTF
	pop
	pop
	Console.NewLine
	Console.Write "Inverse sine, cosine and tangent of 30 DEG: "
	asin
	syscall SYSCALL_PUTF
	pop
	acos 
	syscall SYSCALL_PUTF
	pop
	atan 
	syscall SYSCALL_PUTF
	pop
	pop
	Console.NewLine
	Console.Write "Square root of 56: "
	push 56.0f
	push 0.5f
	fpow
	syscall SYSCALL_PUTF
	Console.NewLine
	Console.Write "Doing a modulo operation. N=27.6, D=4.5: "
	push 27.6
	push 4.5
	fmod
	syscall SYSCALL_PUTI
	Console.WaitKey
	; VM clears the stack...
	System.Quit 0
}
end
