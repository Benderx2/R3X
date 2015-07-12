native(myfunc, "./mylib.so", 1)

function main(0) 
	print "calling function from mylib.so"
	@myfunc(45)
	print "bye"
	end
endf
