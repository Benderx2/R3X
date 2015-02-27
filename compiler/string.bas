function strlen(1)
	let i = 0
	let s = $1
	while int8_ptr(s+i) != 0
		let i = i+1
	endw
	return i
endf

function strcmp(2)
	let str01 = $1
	let str02 = $2
	let i = 0
	// If they are if different length, they are obviously not equal
	if @strlen(str01) != @strlen(str02) return 1
	while i <= @strlen(str01)
		if int8_ptr(str01+i) != int8_ptr(str02+i) return 1
		let i = i + 1
	endw
	return 0
endf
function main(0)
	let is_equal_or_not = @strcmp("hello", "hello")
	if is_equal_or_not = 0 goto yay
	print "string doesnt work"
	end
:yay
	print "string works"
	end
endf
