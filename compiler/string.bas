function strlen(1)
	let i = 0
	let s = $1
	while (int8_ptr(s+i) != 0)
		let i = i+1
	endw
	return i
endf
function strcpy(2) 
	let Source = $1
	let Destination = $2
	let i = 0
	while(0 = 0)
		if(int8_ptr(Source+i) = 0) goto finish
		let int8_ptr(Destination+i) = int8_ptr(Source+i)
		let i = i + 1
	endw
:finish
	return 0
endf
function strcmp(2)
	let str01 = $1
	let str02 = $2
	let i = 0
	if (@strlen(str01) != @strlen(str02)) return 1
	while(i <= @strlen(str01))
		if (int8_ptr(str01+i) != int8_ptr(str02+i)) return 1
		let i = i + 1
	endw
	return 0
endf
function main(0)
	let is_equal_or_not = @strcmp("hello", "hello")
	if (is_equal_or_not = 0) goto yay
	print "string doesnt work"
	end
:yay
	print "string works"
	let x = @strlen("hello")
	let x = alloc(7)
	@strcpy("hello", x)
	print "strcpy: "; x
	print $x
	end
endf
