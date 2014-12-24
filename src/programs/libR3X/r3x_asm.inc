use32
define SYSCALL_PUTS 0x0 
define SYSCALL_PUTI 0x1 
define SYSCALL_PUTF 0x2
define SYSCALL_GLUPDATE 0x3
define SYSCALL_GETC 0x4
define SYSCALL_PUTCH 0x5
define SYSCALL_ATOI 0x6
define R0 0
define R1 1
define R2 2 
define R3 3 
define R4 4	
macro push op {
		db 0x01
		dd op
}
macro syscall op {
	db 0x21
	db op
}
macro cmp {
	db 0x0B
}
macro pop op {
	db 0x02
}
macro add {
	db 0x03
}
macro sub {
	db 0x04
}
macro mul {
	db 0x05
}
macro div {
	db 0x06
}
macro fadd {
	db 0x07
}
macro fsub {
	db 0x08
}
macro fmul {
	db 0x9
}
macro fdiv {
	db 0xA	
}
macro jmp op {
	db 0x20
	dd op
}
macro je op {
	db 0x0C
	dd op
}
macro jg op {
	db 0x0E
	dd op
}
macro jl op {
	db 0x0D
	dd op
}
macro jz op {
	db 0x11
	dd op
}
macro slp {
	db 0x0
}
macro exit {
	db 0x1F
}
macro dup {
	db 0x15
}
macro load {
	db 0x18
}
macro loads off {
	db 0x17
	dd off
}
macro or op {
	db 0x13
	dd op
}
macro xor op {
	db 0x14
	dd op
}
macro and op {
	db 0x12
	dd op
}
macro loadlib {
	db 0x22
}
macro libexec {
	db 0x23
}
macro store {
	db 0x19
}
macro call op {
	db 0x24
	dd op
}
macro ret {
	db 0x25
}
macro pusha op {
	db 0x26
	dd op
}
macro popa {
	db 0x27
}
macro memcpy {
	db 0x28
}
macro lodsb {
	db 0x29
}
macro lodsd {
	db 0x31
}
macro stosb {
	db 0x2A
}
macro stosd {
	db 0x2E 
}
;macro cmpsb {
;	0x2F
;}
;macro cmpsd {
;	0x30 
;}
macro loadr reg, op {
	db 0x2B
	db reg
	dd op
}
macro pushr reg {
	db 0x2C
	db reg
}
macro popr reg {
	db 0x2D
	db reg 
}
macro pushar reg {
	db 0x38 
	db reg
}
macro popar reg {
	db 0x39
	db reg
}
macro neg { 
	db 0x37
}
macro not {
	db 0x36
}
macro incr reg {
	db 0x32
	db reg
}
macro decr reg {
	db 0x33
	db reg
}
macro cmpr reg1, reg2 {	
	pushr reg1
	pushr reg2
	cmp
	popr reg2
	popr reg1
}
macro int num {
	db 0x34
	db num
}
macro loadi num, reg {
	db 0x35
	db num
	dd reg
}
macro pushstring string {
	local addr, behind
	push addr
	jmp behind
	addr: db string, 0
	behind:
}
