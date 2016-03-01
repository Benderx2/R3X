/*
Copyright (c) 2015 Benderx2, 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of R3X nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef R3X_OPCODES_H
#define R3X_OPCODES_H
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
#define R3X_LODSW 0x62
#define R3X_LODSD 0x31
#define R3X_STOSB 0x2A
#define R3X_STOSD 0x2E
#define R3X_STOSW 0x61
#define R3X_CMPSB 0x2F
#define R3X_CMPSW 0x63 
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
#define R3X_FSIN 0x56
#define R3X_FCOS 0x57
#define R3X_FTAN 0x58
#define R3X_ASIN 0x59
#define R3X_ACOS 0x5A
#define R3X_ATAN 0x5B
#define R3X_FPOW 0x5C
#define R3X_MOD 0x5D
#define R3X_FMOD 0x5E
#define R3X_RCONV 0x5F
#define R3X_ACONV 0x60
#define R3X_CMPS 0x67
#define R3X_POPN 0x68
#define R3X_PUSHF 0x69
#define R3X_POPF 0x6A
#define R3X_TERN 0x6B
#define R3X_CATCH 0x6C
#define R3X_THROW 0x6D
#define R3X_HANDLE 0x6E
#define R3X_STORES 0x6F
#define R3X_LOADSR 0x70
#define R3X_STORESR 0x71
#define R3X_SETE 0x72
#define R3X_SETNE 0x73
#define R3X_SETG 0x74
#define R3X_SETL 0x75
#define R3X_FSINH 0x76
#define R3X_FCOSH 0x77
#define R3X_FTANH 0x78
#define R3X_FABS 0x79
#define R3X_FLOOR 0x7A
#define R3X_CEIL 0x7B
#define R3X_ASINH 0x7C
#define R3X_ACOSH 0x7D
#define R3X_ATANH 0x7E
#define R3X_FCONV 0x7F
#define R3X_ICONV 0x80
#define R3X_FSQRT 0x81
#define R3X_JMPL 0x82
#define R3X_JEL 0x83
#define R3X_JGL 0x84
#define R3X_JLL 0x85
#define R3X_JZL 0x87
#define R3X_PUSHIP 0x86
#define R3X_REX64_INST01 0x88
#define R3X_REX64_INS02
#define R3X_ARS 0x89
#define R3X_BREAK 0x8A
#define R3X_CALLL 0x8B
#define RFC_PREFIX 0x8C
#define R3X_LOR 0x8D
#define R3X_LAND 0x8E
// RFC (REX Fast Computing)
#define RFC_LOADR64 0
#define RFC_PUSH64 1
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
#define SYSCALL_LOADDYNAMIC 0x9
#define SYSCALL_OPENSTREAM 0xA
#define SYSCALL_SEEKSTREAM 0xB
#define SYSCALL_CLOSESTREAM 0xC
#define SYSCALL_READSTREAM 0xD
#define SYSCALL_WRITESTREAM 0xE
#define SYSCALL_TELLSTREAM 0xF
#define SYSCALL_FREE 0x12
#define SYSCALL_GETARGS 0x13
// Non-standard stuff
#define SYSCALL_GETCPUCLOCK 0x10 
#define SYSCALL_GETCLOCKSPERSEC 0x11
#endif
