function main, 0
	print "Array stuff"
	goto mylabel
:mylabel
// Allocate 4 bytes for S.
	let buffer = alloc(4)
	let B=100
// int32_ptr = 32-bit array access, int16_ptr = 16-bit array access, int8_ptr = 8-bit array access
// 122233 = Decimal 0xFFF2FFF = Hex, 0b10101111 = Binary, 0o23339 = Octal
	let int32_ptr(buffer) = 1098143098+0xFF
	let X = int16_ptr(buffer)
	print "(uint16_t)S[0] and (uint32_t)S[0] = "; X, int32_ptr(buffer)
	let mystr = "Hello, World!"
	print $mystr
	let firstvariable = int8_ptr(mystr)
	print "first char: "; firstvariable
	let S = 0
	end
endf
