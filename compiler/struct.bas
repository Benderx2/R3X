#define exec_load_addr 0x100000
struct mystruct(1)
	int32 header
ends
struct mystruct02(3)
	int8 member1
	int16 member2
	int32 member3
ends
function main(0)
	let myvar = alloc(sizeof(mystruct02))
	let <struct mystruct02>myvar.member1 = 0x34
	let <struct mystruct02>myvar.member2 = 32768
	let <struct mystruct02>myvar.member3 = 0xDEADBEEF
	if (<struct mystruct>(exec_load_addr).header = 0xBA5EBA11) goto success
	print "not loaded using compliant header"
	end
:success
	print "compliant header detected!"
	print "size of mystruct02: "; sizeof(mystruct02)
	print "member 1 ="; <struct mystruct02>myvar.member1
	print "member 2 ="; <struct mystruct02>myvar.member2
	print "member 3 ="; <struct mystruct02>myvar.member3
	end
endf
