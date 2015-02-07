#! No scoping yet. :'(
function x, 3
	print "I'm a function take takes 3 args"
	let a = $1
	print "the number got in the first arg is: "; a
	let b = $2+15
	print "the number got in the second arg + 15 is :"; b
	return $3
endf

function main, 0
	let c = 24 
	let retval = @x(100, 200, c * 5 + 2)
	print "return value: "; retval
	if retval = c*5+2 goto correct
	print "lol, this failed."
	end
:correct
	print "lol it's right"
	end
endf
