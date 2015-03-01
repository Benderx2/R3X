function factorial(1)
	let number = $1
	if (number <= 1) goto func_return
	return number*@factorial(number-1)
:func_return
	return 1
endf

function main(0)
	let fact = @factorial(9)
	print "factorial of 9: "; fact 
	end
endf
