#ifndef __R3X_OPCODES_H
#define __R3X_OPCODES_H
#define R3X_SLEEP 0x0
#define R3X_PUSH 0x01
#define R3X_POP 0x02
#define R3X_ADD 0x03
#define R3X_SUB 0x04
#define R3X_MUL 0x05
#define R3X_DIV 0x06
#define R3X_FADD 0x07
#define R3X_FSUB 0x08
#define R3X_FMUL 0x09
#define R3X_FDIV 0xA
#define R3X_CMP 0x0B
#define R3X_JE 0x0C
#define R3X_JL 0x0D
#define R3X_JG 0x0E
#define R3X_JZ 0x11
#define R3X_JMP 0x20
#define R3X_AND 0x12
#define R3X_OR 0x13
#define R3X_XOR 0x14
#define R3X_DUP 0x15
#define R3X_LOADS 0x17
#define R3X_LOAD 0x18
#define R3X_STORE 0x19
// 0x1A Reserved
#define R3X_EXIT 0x1F
#define R3X_SYSCALL 0x21
#define R3X_LOADLIB 0x22
#define R3X_LIBEXEC 0x23
#define R3X_CALL 0x24
#define R3X_RET 0x25
#define R3X_PUSHA 0x26
#define R3X_POPA 0x27
#define R3X_MEMCPY 0x28
#define R3X_LODSB 0x29
#define R3X_LODSD 0x31
#define R3X_STOSB 0x2A
#define R3X_STOSD 0x2E
#define R3X_CMPSB 0x2F
#define R3X_CMPSD 0x30
#define R3X_LOADR 0x2B
#define R3X_PUSHR 0x2C
#define R3X_POPR 0x2D
#define R3X_INCR 0x32
#define R3X_DECR 0x33
#define R3X_INT 0x34
#define R3X_LOADI 0x35
#define R3X_NOT 0x36
#define R3X_NEG 0x37
#define R3X_PUSHAR 0x38
#define R3X_POPAR 0x39
#define R3X_SHR 0x4A 
#define R3X_SHL 0x4B
#define R3X_ROR 0x4C
#define R3X_ROL 0x4D
#define R3X_CALLDYNAMIC 0x53
// Relocatable instructions
#define R3X_LODSB_RELOC 0x4E
#define R3X_LODSD_RELOC 0x4F
#define R3X_STOSB_RELOC 0x50
#define R3X_STOSD_RELOC 0x51
#define R3X_PUSH_RELOC 0x52
// 0x53
// Syscalls
#define SYSCALL_PUTS 0x0
#define SYSCALL_PUTI 0x1
#define SYSCALL_PUTF 0x2
#define SYSCALL_GLUPDATE 0x3
#define SYSCALL_GETC 0x4
#define SYSCALL_PUTCH 0x5
#define SYSCALL_ATOI 0x6
#define SYSCALL_ALLOC 0x7
#define SYSCALL_DISPATCH 0x8
#define SYSCALL_LOADDYNAMIC 0x9
#endif
