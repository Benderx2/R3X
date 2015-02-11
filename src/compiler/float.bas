function main, 0
	print "floating point test"
	#! sorry bros and sis, due to this typelessness, you just fkn use different operators.
	let A = mul_f(11.5, add_f(30.567, 23.01))
	#! round off and convert to int32.
	let rounded_a = conv_i(A)
	#! can convert the rounded value back too. tho the mantissa will be lost.
	let rounded_a_as_float = conv_f(rounded_a)
	#! use '%' prefix for print float stuff
	print "result of adding 30.567, 23.01 and the multiplying with 11.5 and rounding it off as int32 "; %A, rounded_a
	end
endf
