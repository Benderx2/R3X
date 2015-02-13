function main, 0
	print "Testing Division and bitwise"
	goto mylabel
	print "Testing division"
:mylabel
	let X = 21
	let Y = 5
	let Z = X / Y
	let R = X%Y
	print "Mod: "; R
	print "Div: "; Z
	let A = 50 & 2
	let Q = 50<<2
	#! Always add a '1' before a neg (~) or a not (!) !
	let C = 1!Q
	print "Or: "; A
	print "Q, C = "; Q, C
	input Y
	print "Y = "; Y
	print Y
	asm "push 0"
	asm "exit"
endf
