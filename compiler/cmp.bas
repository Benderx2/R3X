function main(0) 
	let a = (5 <= 8) && (5 := 6)
	print "Checking if 5 is less than 8 and 5 is equal to 6 lol. (Should be 0): " ; a
	let qvar = 1
	let ivar = 3
	if((qvar := 1) && (ivar != 2)) goto we_are_cool
	end
:we_are_cool
	print "we are cool!"
	end
endf
