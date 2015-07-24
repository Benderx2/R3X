#include <rstdlib.h>

function main(0)
	print "Hello, World!"
	print "Welcome to my First Program"
	print "Type something: (11 characters):"
	let string = alloc(12)
	input string, 12
	print "You typed: "; $string
	end
endf
