/* Compile as: t++ -s test_func_lib.bas -o test_func.ro -d */
function addnumbers(2)
	let A = $1
	let B = $2
	let C = A+B
	return C
endf
function test_func(0)
	print "hello, world from ro!"
	let num = @addnumbers(58,62)
	print "result: "; num
	return 0
endf
