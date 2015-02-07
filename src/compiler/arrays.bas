function main, 0
	print "Array stuff"
	goto mylabel
:mylabel
#! Allocate 4 bytes for S.
	alloc buffer = 4
	let B=100
#! {} = 32-bit array access, <> = 16-bit array access, [] = 8-bit array access
#! 122233 = Decimal 0xFFF2FFF = Hex, 0b10101111 = Binary, 0o23339 = Octal
	let {buffer} = 0xA10F2CD+0x100ABC+(0b1010111001 + 0o1703+0x01)
	let X = <buffer>
	print "(uint16_t)S[0] and (uint32_t)S[0] = "; X, {buffer}
	let mystr = "Hello, World!"
	print $mystr
	let firstvariable = [mystr]
	print "first char: "; firstvariable
	let S = 0
	end
endf
