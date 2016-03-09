#include <rstdlib.h>

function main(0)
	print "press a key and this program will react. press Q and it will quit"
	let a = 0 
:check_key
	let a = @r_getc()
	if(a := 0) goto check_key
	if((a := 'Q') || (a := 'q')) asm "exit"
	print "you pressed key no.:"; a
	goto check_key
endf
