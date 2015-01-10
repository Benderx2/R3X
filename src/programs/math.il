include 'libR3X/libR3X.pkg'
.text {	
	function main
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
	syscall SYSCALL_PUTF
	popn 3
	Console.NewLine
	Console.Write "Finding sinh, cosh, tanh, asinh, acosh, atanh of 30 DEG: "
	push 30.0f
	rconv
	fsinh
	syscall SYSCALL_PUTF
	pop
	fcosh 
	syscall SYSCALL_PUTF
	pop
	ftanh
	syscall SYSCALL_PUTF
	pop
	asinh
	syscall SYSCALL_PUTF
	pop
	acosh
	syscall SYSCALL_PUTF
	pop
	atanh
	syscall SYSCALL_PUTF
	pop
	Console.NewLine
	Console.Write "Doing a floor/ceil operation on 30.5555: "
	push 30.5555f
	floor
	syscall SYSCALL_PUTF
	pop
	ceil 
	syscall SYSCALL_PUTF
	pop
	Console.NewLine
	Console.Write "Converting 30.567 to a 32-bit integer: "
	push 30.567
	iconv
	syscall SYSCALL_PUTI
	Console.WaitKey
	; VM clears the stack...
	System.Quit 0
	endfunction main
}
end
