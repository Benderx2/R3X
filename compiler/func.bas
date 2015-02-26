/* this is a global variable */
global myglobal
function x, 3
	let i = add_f(add_f(2.4, mul_f(3.5, 4.6)), 5.6)
	print "float i = "; %i
	print "I'm a function take takes 3 args"
	let a = $1
	print "the number got in the first arg is: "; a
	/* NOTE: $2 + 15 is something different than the below
	$2 + 15 means load the 17th argument! This is largely different
	from load the 2nd argument and add 15! */
	let b = $2
	let b = b + 15
	print "the number got in the second arg + 15 is :"; b
	let f = 5
	print "f in this scope is: "; f
	return $3
endf
function add2numbers, 2
	let a = $1
	let b = $2
	return a + b
endf
function subtract2numbers, 2
	let a = $1
	let b = $2
	return a - b
endf
function misc, 0
	let f = 0
	print "value of global variable"; <global>myglobal
	print "f in this scope is"; f
	/* Dont FORGET TO RETURN! else cause undefined behaviour lol */
	return 0
endf
function main, 0
	let <global>myglobal = 5
	let xxx = 0
	let y_z = 0
	let f = 1
	@misc()
	let c = 24
	let retval = @x(100, 200, c * 5 + 2)
	print "return value: "; retval
	if retval = c*5+2 goto correct
	print "lol, this failed."
	end
:correct
	print "lol it's right"
	print "f in this scope is: "; f
	print "adding 2 numbers: 57 and 90"
	let result = @add2numbers(57, 90)
	print "result = "; result
	print "subtracting 2 numbers: 30 and 10"
	let result = @subtract2numbers(30, 10)
	print "result = "; result
endf
