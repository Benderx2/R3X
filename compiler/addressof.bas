function x(0)
	print "hello, world from x!"
	return 0
endf
function y(2)
	let FirstArgument = $1
	let SecondArgument = $2
	print "first arg: "; FirstArgument
	print "second arg: "; SecondArgument
	return 5
endf
function main(0)
	let a = addressof(@x)
	let q = addressof(a)
	/* Call address a with 0 arguments */
	@raw_call(a, 0)
	let a = addressof(@y)
	/* Call address a with 3 arguments */
	let c = @raw_call(a, 2, 15, 27+22)
	print "return value from y ="; c
	end
endf
