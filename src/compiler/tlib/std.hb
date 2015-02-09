function strlen, 1
	let i = 1
	while [$1+i] != 0
		i = i+1
	endw
	return i
endf

function strcmp, 2
	let str01 = $1
	let str02 = $2
	let i = 0
	while i < @strlen(str01)
		if [str01+0] != [str02+i] return 1
		i = i + 1
		if i > @strlen(str02) return 1
	endw
	return 0
endf
