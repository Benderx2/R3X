include 'r3x_asm.inc'
format binary as 'r3x'
define R3_ERROR_CODE 0xFFFFFFFF
; File functions
define SEEK_SET 0
define SEEK_CUR 1
define SEEK_END 2
define M_PI 3.141592f
define R3_LOAD_POINT 0x100000
define VIDEO_MEMORY_START 512
org 0x100000
_begin_r3x_kernel:
dd 0xBA5EBA11
dd _init
dd _end_text - _init
dd _data
dd _end_data - _data
dd _bss
dd _end_bss - _bss
dd 0x0
dd _end_bss - _begin_r3x_kernel
db 'ABCDEFG', 0
db 'PUBLISH', 0
define R3X_LIB_VERSION 0x10C
define SYS_NEWLINE 0xD
define R3X_NEWLINE 0xA
macro DECLARE_SECTION name {
      local m,c                       ;avoid name clashing
        c equ 0                         ;initialise the counter
     macro .#name [a] \{
           \common
            rept 1 x:c+1 \\{
                 c equ x         ;update the counter
                 macro m\\#x a ;open the macro
             \\}
  \}
    macro STORE_SECTION_#name \{
           rept c x \\{
                 m\\#x         ;replay each line
           \\}
  \} 
}
DECLARE_SECTION text
DECLARE_SECTION data
DECLARE_SECTION bss
macro end {
	_init:
	STORE_SECTION_text
	_end_text:
	_data:
	STORE_SECTION_data
	_end_data:
	_bss:
	STORE_SECTION_bss
	_end_bss:
}
macro pushstring string {
	local addr, behind
	push addr
	.data \{ addr: db string, 0 \}
} 
; Console.Write -- Writes a string using syscall puts
macro Console.Write str {
	pushstring str 
	syscall SYSCALL_PUTS 
	pop
}
; Write a floating point integer to console
macro Console.WriteFloat {
	syscall SYSCALL_PUTF 
}
macro Console.WriteInt {
	syscall SYSCALL_PUTI
}
; Writes a newline to Console
macro Console.NewLine {
	push 0xA
	syscall SYSCALL_PUTCH
	pop
}
; Writes a string from the pointer
macro Console.WritePointer addr {
	push addr
	syscall SYSCALL_PUTS
	pop
}
; Waits for keypress
macro Console.WaitKey {
	call __WaitKey
}
; Inputs string from console
macro Console.Input addr, size {
	pushr R0
	pushr R3
	loadr R0, addr
	loadr R3, size
	call __GetStringFunc
	popr R3
	popr R0
}
; Copies a block of memory
macro Memcpy dest, src, size {
	push dest
	push src
	push size 
	memcpy 
	pop
	pop 
	pop 
}
; Copies a block of memory, but uses registers instead, (R0 = Dest, R1 = src, R2 = Size)
macro MemcpyR {
	pushr R0 
	pushr R1
	pushr R2
	memcpy
	popr R2
	popr R1
	popr R0
}
; Quit the application
macro System.Quit arg {
	push arg
	exit
}
; Load a native library
macro System.LoadLibrary libname {
	pushstring libname
	loadlib
	pop
}
; Call a function from the library
macro System.LibraryCall functioname, libname {
	pushstring libname
	pushstring functioname
	libexec
	pop 
	pop 
}
macro System.AllocateMemory size {
	push size
	syscall SYSCALL_ALLOC
	pop
}
macro System.DispatchJob ip { 
	push ip
	syscall SYSCALL_DISPATCH 
	pop
}
; Stores a value to address
macro StoreInt addr,  integer {
	pushr R0
	pushr R1
	loadr R0, addr
	loadr R1, var
	stosd
	popr R1
	popr R0
}
; Stores a byte to address
macro StoreByte addr, byten {
	pushr R0
	pushr R1
	loadr R0, addr
	loadr R1, var
	stosb
	popr R1
	popr R0
}
macro Stack.Push pushval {
	push pushval
}
macro Stack.Pop {
	pop 
}
macro Stack.PushRegister reg {
	pushr reg
}
macro Stack.PopRegister reg {
	popr reg
}
; INTERNAL FUNCTIONS -- NOT FOR APPLICATION USE.
__GetStringFunc:
	pushr R0
	pushr R1
	pushr R2
	loadr R1, 0
	loadr R2, 0
	.loop1:
		; Get Character
		syscall SYSCALL_GETC ; PU 1
		; Check if it's 0
		push 0 ; PU 2
		cmp 
		; Pop out our value
		pop ; PO 1
		je .is_null
		; Loop 
		; Check if it's a newline, else 
		push SYS_NEWLINE ; PU 3
		cmp
		pop ; PO 2
		popr R1
		; then end...
		je .end
		; Else put character.
		;syscall SYSCALL_PUTCH
		; Pop the character into R1
		pushr R1
		syscall SYSCALL_PUTCH
		; Store it
		stosb
		popr R1
		; Increment Index (R0)
		pushr R0 ; PU 4
		push 1 ; PU 5
		add ; PU 6
		; Pop R0 
		popr R0 ; PO 4
		pop ; PO 5
		pop ; PO 6
		; Incrment Counter (R2)
		pushr R2 ; PU 7
		push 1 ; PU 8
		add ; PU 9
		popr R2 ; PO 7
		pop ; PO 8
		pop ; PO 9
		; Check if R2 == Size..
		pushr R2 ; PU 10
		pushr R3 ; PU 11
		cmp
		pop 	; PO 10
		pop    ; PO 11
		; End then..
		je .end
		; Else loop
		jmp .loop1
	.is_null:
		pop
		jmp .loop1
	.end:
		popr R2
		popr R1
		popr R0
		ret
__WaitKey:
	syscall SYSCALL_GETC
	.loop_key:
		syscall SYSCALL_GETC
		push 0
		cmp
		pop
		pop
		je .loop_key
		ret
