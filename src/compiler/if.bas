function main, 0
	print "If stuff"
	#! Allocate 256 bytes for buffer
	alloc buffer = 256
	let int16_ptr(buffer) = 0xFF44
	if int8_ptr(buffer) = 0x44 goto yes
	print "Uh, error!"
	end
:yes
	print "Yay, little endian!"
	end
endf
