print "If stuff"
#! Allocate 256 bytes for buffer
alloc buffer = 256
let <buffer> = 0xFF44
if [buffer] = 0x44 goto yes
print "Uh, error!"
end
label yes
print "Yay, little endian!"
end
