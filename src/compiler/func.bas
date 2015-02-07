function x, 3
	print "I'm a function take takes 3 args"
	let a = $1
	print "the number got in the first arg is: "; a
	let b = $2+15
	print "the number got in the second arg + 15 is :"; b
	return $3
endf
function main, 0
	let a = 24 
	let b = @x(100, 200, a * 50)
	if b = (a*50) goto correct
	print "lol, this failed."
	end
:correct
	print "lol it's right"
	end
endf
