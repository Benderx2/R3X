function factorial, 1
	if $1 <= 1 goto func_return
	return $1*@factorial($1-1)
:func_return
	return 1
endf

function main, 0
	let fact = @factorial(9)
	print "factorial of 9: "; fact 
	end
endf
