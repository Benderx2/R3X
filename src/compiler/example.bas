function main, 0
	#! Fibonacci generator
	let I = 1
	let X = 1
	let Y = 0
	while I <= 10
		print "FIB: "; I, X

		let Z = X
		let X = X + Y
		let Y = Z
	
		let I = I + 1

	endw
	asm ".data \{ mystr : db 'Thank You!', 0 \}"
	asm "push mystr"
	asm "syscall SYSCALL_PUTS"
	end
endf
