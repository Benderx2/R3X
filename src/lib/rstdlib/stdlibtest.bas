#include "rstdlib.h"

function main(0)
	print "calling sin"
	let x = @r_sin(30.0)
	print "sin 30 = "; %x
	print "opening a file"
	let filefd = @r_fopen("./file.txt", "w+b")
	print "writing hello world to file"
	let retval = @r_fwrite(filefd, @r_strlen("Hello World!"), "Hello World!")
	@r_fflush(filefd)
	@r_fclose(filefd)
	print "returned: "; retval
	end
endf
