/* Compile as: t++ -s hello.bas -o hello.exe */
extern (test_func, "test_func.ro", 0)
function main(0)
	 @test_func()
	print "Hello World!" 
	let a = 5
	if ( a := 5 ) print "a is 5!"
	print "press any key to continue!"
	end
endf
