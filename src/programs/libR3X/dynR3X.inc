include 'r3x_asm.inc'
format binary as 'ro'
org 0
dd 0xDEADBEEF
dd _text
dd _end_text - _text
dd _export
dd _end_export - _export
dd _data
dd _end_data - _data
dd _bss 
dd _end_bss - _bss
macro pushad val {
	db 0x52
	dd val
}
macro DECLARE_SECTION name {
        local m,c
        c equ 0
        macro .#name [a] \{ 
                \common 
                rept 1 x:c+1 \\{ c equ x
                                 macro m\\#x a \\}
        \} 
        macro STORE_SECTION_#name \{ 
                rept c x \\{ m\\#x \\}
        \} 
}
DECLARE_SECTION text
DECLARE_SECTION data
DECLARE_SECTION bss
DECLARE_SECTION export
macro end {
	_text:
	STORE_SECTION_text
	_end_text:
	_export:
	STORE_SECTION_export
	_end_export:
	_data:
	STORE_SECTION_data
	_end_data:
	_bss:
	STORE_SECTION_bss
	_end_bss:
}
macro pushstring string {
	local addr, behind
	pushaddr addr
	.DATA \{ addr: db string, 0 \}
} 
macro stosd {
	db 0x2E
	dd 0
}
macro stosb {
	db 0x2A
	dd 0
}
macro lodsd {	
	db 0x31
	dd 0
}
macro lodsb {
	db 0x29
	dd 0
}
macro lodsw {
	db 0x62
	dd 0
}
macro cmpsw {
	db 0x63
	dd 0
}
macro stosw {
	db 0x61
	dd 0
}
macro load {
	display "Error: Attempt to use instruction 'load' in dynamic library. Instruction Not supported"
	err
}
macro store {
	display "Error: Attempt to use instruction 'store' in dynamic library. Instruction Not supported"
	err
	
}
macro EXPORT_FUNCTION function_id, function_pointer { 
	dd function_id
	dd function_pointer
}
