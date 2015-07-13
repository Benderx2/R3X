native(myfunc, "./mylib.so", 1)

function main(0) 
	print "calling function from mylib.so"
	let asshole = @myfunc("Hello, This is the string from the main function!")
	print "i got (hopefully that evil number):"; asshole
	let myshit = 6 + 322 + 1139
	let stuff = myshit * 6
	print "stuff: "; stuff
	end
endf
